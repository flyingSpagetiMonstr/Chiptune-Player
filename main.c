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
uint8_t flag1 = 0; // 中断标志位，每次按键产生一次中断，并开始读取8个数码管的值
uint8_t Rx2_Buffer[8]={0};
uint8_t Tx1_Buffer[8]={0};
// uint8_t Rx1_Buffer[1]={0};

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

#define WARM_ENABLED

#include "presets.c"
#include "scores.c"
#include "data.c"

#define FREQ_DIV ((int)1e6)
#define FACTOR ((int)(FREQ_DIV/((int)(1e3)))) // FREQ_DIV / 1e3
#define REST_RATE 5
// #define REST_RATE 0.05
// #define UNIT (1/FREQ_DIV)

#define TRANSLATE(read) (0x1C - (read) + 1)

void play_sound(sound s);
void play_score(sound *sounds, int sounds_len);

void state_wait(void);

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
    play_score(anthem, 5);
    // play_score(anthem, len);
    // play_score(BWV846, BWV846_len);
    // play_score(Ievan_polkka, Ievan_polkka_len);

#ifdef WARM_ENABLED
	if(warm == WARM_MARK)
	{
		// warm boot
		// check_backup(&stat_bkp1, &stat_bkp2, &stat_bkp3, 1);
		// check_backup(&func_bkp1, &func_bkp2, &func_bkp3, 1);
	}
	else
	{
		// cold boot
		warm = WARM_MARK;
		init_data();
	}

#endif



#if 1
	uint8_t state = STATE(read_data());
	while (1)
	{
		switch (state)
		{
		case WAIT:
			state_wait();
			break;
		default: // case PLAY1 PLAY2 PLAY3
			// state_play:
			sound* score = scores[state - 1];
			uint16_t len = lens[state - 1];
			play_score(score, len);
			break;
		}
	}

#else
    while (1)
    {
        if(flag1 == 1)
        {
            flag1 = 0;
            I2C_ZLG7290_Read(&hi2c1, 0x71, 0x01, Rx1_Buffer, 1); //读键值
            DC_Task(Rx1_Buffer[0]);
        }
        Led(DC_Motor_Data);
            
        HAL_Delay(100* FACTOR);
    }
#endif
}

void play_sound(sound s)
{
    uint16_t period = periods[s.note];
    uint16_t half_p = period / 2;

    // uint32_t duration = s.duration / UNIT;
    // uint32_t duration = s.duration * FREQ_DIV;
    uint32_t start_time = HAL_GetTick();

    if (s.note == 0)
    {
        HAL_Delay(s.duration);
        return;
    }

    // while ((HAL_GetTick() - start_time) < s.duration * (1 - REST_RATE)) // ###
    while ((HAL_GetTick() - start_time) < (s.duration / 100) * (100 - REST_RATE)) // ###
    {
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
        HAL_Delay(half_p);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_Delay(half_p);
    }
    // HAL_Delay(s.duration * REST_RATE); // ###
    HAL_Delay((s.duration / 100) * 5); // ###
}

void play_score(sound *sounds, int sounds_len)
{
    for (uint16_t i = 0; i < sounds_len; i++)
    {
        play_sound(sounds[i]);
    }
}

void state_wait(void)
{
    while (1)
    {
        if(flag1 == 1)
        {
            flag1 = 0;

			uint8_t key_read;
            I2C_ZLG7290_Read(&hi2c1, 0x71, 0x01, &key_read, 1); //读键值
			key_read = TRANSLATE(key_read);
			if (!(1 <= key_read && key_read <= 3))
			{
				continue;
			}

			store_state(key_read);
        }
        HAL_Delay(100* FACTOR);
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

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/FREQ_DIV); // modified
	// HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// OUR CODES ENDS HERE
// ===================================================================


void Led(int date) //显示函数
{
/*****************数据转换*****************************/ 
unsigned int qian,bai,shi,ge;
  qian=date/1000;				 //求千位
  bai=date%1000/100;		 //求百位
  shi=date%1000%100/10;			 //求十位
  ge=date%10;						 //求个位	
	
//	I2C_Gets(ZLG7290_I2C_ADDR,0x10,1,Rx2_Buffer,8);//读8位数码管
	
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
