#ifndef SCORES_C
#define SCORES_C

// ============================================================================================
// SCORE DATA

const uint16_t periods[] = {0, 15289, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012};

typedef struct _sound
{
    uint8_t note;
    uint32_t duration;
} sound;


#define _BWV846
#define IEVAN_POLKKA

sound anthem[] = {
    {40, 2400000}, { 0,  200000}, {35,  200000},
    {40,  800000}, {35,  600000}, {37,  200000}, {39,  800000}, {32,  400000}, {32,  400000},
    {37,  800000}, {35,  600000}, {33,  200000}, {35,  800000}, {28,  400000}, {28,  400000},
    {30,  800000}, {30,  600000}, {32,  200000}, {33,  800000}, {33,  600000}, {35,  200000},
    {37,  800000}, {39,  400000}, {40,  400000}, {42, 1200000}, {35,  400000},
    {44,  800000}, {42,  600000}, {40,  200000}, {42,  800000}, {39,  400000}, {35,  400000},
    {40,  800000}, {39,  600000}, {37,  200000}, {39,  800000}, {32,  400000}, {32,  400000},
    {37,  800000}, {35,  600000}, {33,  200000}, {35,  800000}, {28,  600000}, {28,  200000},
    {40,  800000}, {39,  600000}, {37,  200000}, {35, 1200000}, { 0,  400000},
    {00, 2000000}
};
uint16_t const anthem_len = sizeof(anthem)/sizeof(sound);

#ifdef _BWV846
sound BWV846[] = {
    {25,  200000}, {29,  200000}, {32,  200000}, {37,  200000}, {41,  200000}, {32,  200000}, {37,  200000}, {41,  200000},
    {25,  200000}, {29,  200000}, {32,  200000}, {37,  200000}, {41,  200000}, {32,  200000}, {37,  200000}, {41,  200000},
    {25,  200000}, {28,  200000}, {33,  200000}, {38,  200000}, {42,  200000}, {33,  200000}, {38,  200000}, {42,  200000},
    {25,  200000}, {28,  200000}, {33,  200000}, {38,  200000}, {42,  200000}, {33,  200000}, {38,  200000}, {42,  200000},
    {24,  200000}, {28,  200000}, {32,  200000}, {38,  200000}, {42,  200000}, {32,  200000}, {38,  200000}, {42,  200000},
    {24,  200000}, {28,  200000}, {32,  200000}, {38,  200000}, {42,  200000}, {32,  200000}, {38,  200000}, {42,  200000},
    {00, 2000000}
};
uint16_t const BWV846_len = sizeof(BWV846)/sizeof(sound);
#endif

#ifdef IEVAN_POLKKA
sound Ievan_polkka[] = {
    {46,  240000}, {46,  240000}, {44,  240000}, {42,  120000}, {42,  120000}, {41,  240000}, {37,  120000}, {37,  360000}, {41,  240000},
    {44,  240000}, {44,  120000}, {44,  120000}, {42,  240000}, {41,  240000}, {42,  240000}, {38,  240000}, {38,  240000}, {42,  240000},
    {46,  120000}, {46,  120000}, {46,  120000}, {46,  120000}, {44,  240000}, {42,  240000}, {41,  240000}, {37,  120000}, {37,  240000}, {37,  120000}, {41,  240000},
    {44,  120000}, {44,  120000}, {44,  240000}, {42,  240000}, {41,  120000}, {41,  120000}, {42,  240000}, {39,  120000}, {39,  480000}, { 0,  120000},
    {34,  240000}, {39,  240000}, {39,  360000}, {41,  120000}, {42,  120000}, {42,  120000}, {39,  120000}, {39,  120000}, {39,  240000}, {42,  120000}, {42,  120000},
    {41,  240000}, {37,  240000}, {37,  240000}, {41,  240000}, {42,  240000}, {39,  240000}, {39,  240000}, {39,  120000}, {39,  120000},
    {34,  240000}, {39,  240000}, {39,  360000}, {41,  120000}, {42,  120000}, { 0,  120000}, {39,  240000}, {39,  120000}, {39,  120000}, {39,  120000}, {42,  120000},
    {46,  120000}, {46,  120000}, {46,  120000}, {44,  120000}, {42,  120000}, { 0,  120000}, {41,  240000}, {42,  240000}, {39,  120000}, { 0,  120000}, {39,  240000}, {39,  120000}, {39,  120000},
    {46,  240000}, {46,  240000}, {44,  240000}, {42,  240000}, {41,  240000}, {37,  120000}, {37,  240000}, {37,  120000}, {37,  120000}, {41,  120000},
    {44,  120000}, {44,  120000}, {44,  120000}, {44,  120000}, {42,  120000}, {42,  120000}, {41,  120000}, {41,  120000}, {42,  240000}, {39,  120000}, {39,  480000}, {39,  120000},
    {46,  240000}, {46,  240000}, {44,  240000}, {42,  240000}, {41,  240000}, {37,  240000}, {37,  120000}, {37,  120000}, {37,  120000}, {41,  120000},
    {44,  120000}, {44,  120000}, {44,  120000}, {44,  120000}, {42,  240000}, {41,  240000}, {42,  240000}, {39,  120000}, {39,  600000},
    {00, 2000000}
};
uint16_t const Ievan_polkka_len = sizeof(Ievan_polkka)/sizeof(sound);

typedef sound *score_t;
score_t scores[3] = {anthem, BWV846, Ievan_polkka};
uint16_t lens[3] = {anthem_len, BWV846_len, Ievan_polkka_len};
#endif

// END OF SCORE DATA
// ============================================================================================

#define FREQ_DIV ((int)1e6) // accurate clock: 1 us
#define FACTOR ((int)1e3) // ratio between original clock (1000) and the accurate clock (FREQ_DIV), i.e., FREQ_DIV / 1000
#define REST_RATE 5

#include "stm32f4xx_hal_iwdg.h"
extern IWDG_HandleTypeDef pIWDG_Handler[1];
extern uint32_t HCLKFreq;


void play_sound(sound s)
{
    uint16_t period = periods[s.note];
    uint16_t half_p = period / 2;

    uint32_t start_time = HAL_GetTick();

    if (s.note == 0)
    {
        HAL_Delay(s.duration);
        return;
    }

    // play for 95% of duration time
    while ((HAL_GetTick() - start_time) < (s.duration / 100) * (100 - REST_RATE)) 
    {
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
        HAL_Delay(half_p);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_Delay(half_p);

        if ((HAL_GetTick() - start_time) % (200 * FACTOR))
        {
            HAL_IWDG_Refresh(pIWDG_Handler);
        }
    }

    // rest for 5% of duration time
    HAL_Delay((s.duration / 100) * REST_RATE);
}

void play_score(sound *sounds, int sounds_len)
{
    HAL_SYSTICK_Config(HCLKFreq/FREQ_DIV); // increase clock accuracy
    HAL_Delay(5 * FACTOR);

    while (1)
    {
        if (flag1) {
            flag1 = 0;
            break;
            // stop playing and return to state WAIT if keyboard interrupt comes
        }

        uint16_t data = read_data_value();
        uint16_t i = INDICATOR(data); // retrieve `i` from data

        if (i >= sounds_len)
        {
            break;
        }

        // refresh digital tube
        if ((i%5) == 0)
        {
            HAL_SYSTICK_Config(HCLKFreq/1000); // restore clock accuracy
            Led(STATE(data));
            HAL_SYSTICK_Config(HCLKFreq/FREQ_DIV); // increase clock accuracy
        }

        play_sound(sounds[i]); 

        inc_indicator();
    }

    HAL_SYSTICK_Config(HCLKFreq/1000); // restore clock accuracy
}


#endif // SCORES_C


