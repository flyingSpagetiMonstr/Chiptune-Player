/* Includes ------------------------------*/
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "zlg7290.h"
#include "stdio.h"
#include "Dc_motor.h"
#include "i2c.h"


/* Private variables ---------------------*/
#define ZLG_READ_ADDRESS1         0x01
#define ZLG_READ_ADDRESS2         0x10

#define ZLG_WRITE_ADDRESS1        0x10
#define ZLG_WRITE_ADDRESS2        0x11
#define BUFFER_SIZE1              (countof(Tx1_Buffer))
#define BUFFER_SIZE2              (countof(Rx2_Buffer))
#define countof(a) (sizeof(a) / sizeof(*(a)))

uint8_t flag; // 不同的按键有不同的标志位值
// uint8_t flag1 = 0; // 中断标志位，每次按键产生一次中断，并开始读取8个数码管的值
uint8_t Rx2_Buffer[8]={0};
uint8_t Tx1_Buffer[8]={0};
uint8_t Rx1_Buffer[1]={0};

unsigned char seg7code[10]={ 0xFC,0x0C,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xE6}; //数码管字根
void Led(int date); 

/* Private function prototypes -----------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* Private function prototypes -----------*/
void swtich_key(void);
void switch_flag(void);


// =============================================================================================
// OUR CODE STARTS HERE
#include "stdlib.h"
#define HAL_IWDG_MODULE_ENABLED

#include "stm32f4xx_hal_iwdg.h"
#include "..\Drivers\STM32F4xx_HAL_Driver\Src\stm32f4xx_hal_iwdg.c"

#include "data.c"
#include "scores.c"

void state_wait(void);
void state_play(uint8_t index);

void seq_check(uint8_t state, uint8_t last_state);
void IWDG_Starter(IWDG_HandleTypeDef *pIWDG_Handler, uint32_t clock, uint32_t overtime);
uint8_t keyboard_read(void);

uint32_t HCLKFreq = 0;
IWDG_HandleTypeDef pIWDG_Handler[1];

int main(void)
{
    /* MCU Configuration----------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_TIM3_Init();
        
    DC_Motor_Pin_Low();

    HAL_TIM_Base_Start_IT(&htim3); //启动定时器3.

    // ========================================================
    // OUR CODE STARTS HERE
    srand((unsigned int)HAL_GetTick());

    if(warm == WARM_MARK)
    {
        // warm boot, doing nothing
    }
    else
    {
        // cold boot
        warm = WARM_MARK;
        reset_data();
        HAL_Delay(100); // 500 is too big, set to 100 now
    }


    HCLKFreq = HAL_RCC_GetHCLKFreq();

    IWDG_Starter(pIWDG_Handler, 4, 5000);
    // 3.77s is too long, set to 0.5s now #########################


    uint8_t state = 0, last_state = -1;
    state = STATE(read_data_value());
    while (1)
    {
        last_state = state;

        Led(state);

        switch (state)
        {
            // `STATE(data)` is altered at the end of each func
            case WAIT: state_wait(); break; 
            default: state_play(state - 1); break; // case PLAY1 PLAY2 PLAY3 
        }

        state = STATE(read_data_value());
        seq_check(state, last_state);
    }
}



void state_wait(void)
{
    while (1)
    {
        while (flag1 != 1) {
            HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
            HAL_IWDG_Refresh(pIWDG_Handler);
            Led(0);
        }
        
        // ################################
        HAL_Delay(rand()%200);

        flag1 = 0;

        uint8_t key_read;
        key_read = keyboard_read();
        translate(&key_read);

        if (1 <= key_read && key_read <= 3)
        {
            store_state(key_read);
            break;
        }
    }

    Led(200); // OK
    HAL_Delay(500);
}

void translate(uint8_t *key_read) 
{
    if (rand()%2)
    {
        switch (*key_read)
        {
            case 26 /*3*/: *key_read = 1; break;
            case 18 /*6*/: *key_read = 2; break; 
            case 10 /*9*/: *key_read = 3; break; 
            default: *key_read = 0; break;
        }
    }
    else
    {
        int const n = 30;
        int tranlate_table[n] = {0};
        tranlate_table[26] = 1;
        tranlate_table[18] = 2;
        tranlate_table[10] = 3;

        if (0 <= *key_read && *key_read < n) *key_read = tranlate_table[*key_read];
        else *key_read = 0;
    }

    return;
}

void state_play(uint8_t index)
{
    sound* score = 0;
    uint16_t len = 0;

    if (index > 2) index = 2; 

    if (rand()%2)
    {
        score = scores[index]; len = lens[index];
    } 
    else
    {
        len = lens[index]; score = scores[index];
    }

    play_score(score, len);

    if (rand()%2)
    {
        Led(200); // OK
        HAL_Delay(500);
        reset_data(); // reset indicator to 0 and store_state(WAIT);
    } 
    else
    {
        Led(200); // OK
        reset_data(); // reset indicator to 0 and store_state(WAIT);
        HAL_Delay(500);
    }
}


void seq_check(uint8_t state, uint8_t last_state)
{
    if (state == last_state
        || (state >= 1 && last_state >= 1))
    {
        warm = 0;
        HAL_NVIC_SystemReset();
    }
}

void IWDG_Starter(IWDG_HandleTypeDef *pIWDG_Handler, uint32_t clock, uint32_t overtime)
{
	pIWDG_Handler->Instance = IWDG;
	pIWDG_Handler->Init.Prescaler = clock;
	pIWDG_Handler->Init.Reload = overtime;
	HAL_IWDG_Init(pIWDG_Handler);
    __HAL_IWDG_START(pIWDG_Handler);
}

uint8_t keyboard_read(void)
{
	uint8_t a = 0, b = 0, c = 0;
	I2C_ZLG7290_Read(&hi2c1, 0x71, 0x01, &a, 1);
	I2C_ZLG7290_Read(&hi2c1, 0x71, 0x01, &b, 1);

	if (a == b && a != 0)
	{
		return a;
	}
	else 
	{
		I2C_ZLG7290_Read(&hi2c1, 0x71, 0x01, &c, 1);
		if (b == c && b != 0)
		{
			return b;
		}
		else if (c ==  a && c != 0)
		{
			return c;
		}
		else
		{
            warm = WARM_MARK;
            HAL_NVIC_SystemReset();
            // return 0xff;
		}
	}
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// OUR CODES ENDS HERE
// ===================================================================


void Led(int date) //显示函数
{
/*****************数据转换*****************************/ 
unsigned int qian,bai,shi,ge;
  qian=date/1000;                //求千位
  bai=date%1000/100;         //求百位
  shi=date%1000%100/10;          //求十位
  ge=date%10;                        //求个位   
        
//  I2C_Gets(ZLG7290_I2C_ADDR,0x10,1,Rx2_Buffer,8);//读8位数码管
        
    if(fs_flag==0) Tx1_Buffer[3] = 0x02;    
    if(fs_flag==1) Tx1_Buffer[3] = 0x00;
    if(fs_flag ==2) Tx1_Buffer[3] =seg7code[0];

    Tx1_Buffer[4] = seg7code[qian];
    Tx1_Buffer[5] = seg7code[bai];
    Tx1_Buffer[6] = seg7code[shi];
    Tx1_Buffer[7] = seg7code[ge];
        
  I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,8);
}


void swtich_key(void)
{
    switch(Rx1_Buffer[0])
    {
        case 0x1C:
                    flag = 1;                   
          break;
        case 0x1B:  
                    flag = 2;
          break;
        case 0x1A:  
                    flag = 3;
          break;
        case 0x14:
                    flag = 4;
          break;   
                case 0x13:
                    flag = 5;
                    break;
        case 0x12:
                    flag = 6;
          break;
        case 0x0C:
                    flag = 7;
          break;
        case 0x0B:
          flag = 8;
          break;
                case 0x0A:
                    flag = 9;
                    break;
                case 0x03:
                    flag = 15;
                    break;
                case 0x19:
                    flag = 10;
                    break;
                case 0x11:
                    flag = 11;
                    break;
                case 0x09:
                    flag = 12;
                    break;
                case 0x01:
                    flag = 13;
                    break;
                case 0x02:
                    flag = 14;
                    break;
        default:
          break;
            }
}

void switch_flag(void){
    switch(flag){
            case 1:
                Tx1_Buffer[0] = 0x0c;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {                                   
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                 
                    }
                break;
            case 2:
                Tx1_Buffer[0] = 0xDA;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);     
                    }
                break;
            case 3:
                Tx1_Buffer[0] = 0xF2;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);     
                    }
                break;
            case 4:
                Tx1_Buffer[0] = 0x66;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                 
                    }
                break;
            case 5:
                Tx1_Buffer[0] = 0xB6;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
            case 6:
                Tx1_Buffer[0] = 0xBE;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
            case 7:
                Tx1_Buffer[0] = 0xE0;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
            case 8:
                Tx1_Buffer[0] = 0xFE;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                  
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                         
                    }
                break;
            case 9:
                Tx1_Buffer[0] = 0xE6;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                  
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                 
                    }
                break;
            case 10:
                Tx1_Buffer[0] = 0xEE;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                  
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
            case 11:
                Tx1_Buffer[0] = 0x3E;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                          
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
                    case 12:
                Tx1_Buffer[0] = 0x9C;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                             
                    }
                break;
                    case 13:
                Tx1_Buffer[0] = 0x7A;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                                 
                    }
                break;
                    case 14:
                            Tx1_Buffer[0] = 0x00;
                            I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,8);
                        break;
                    case 15:
                Tx1_Buffer[0] = 0xFC;
                if(Rx2_Buffer[0] == 0)
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
                    }
                    else
                    {
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);                      
                        I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);                     
                    }
                break;
            default:
                break;
        }
}


int fputc(int ch, FILE *f)
{ 
  uint8_t tmp[1]={0};
    tmp[0] = (uint8_t)ch;
    HAL_UART_Transmit(&huart1,tmp,1,10);    
    return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
