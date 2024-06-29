#ifndef DATA_C
#define DATA_C

// #define WARM_MARK 0x1234ABCD
#define WARM_MARK 0xC0DECAFE

#define _R(data) ((data).value & 0xff)
#define _L(data) ((data).value >> 8)
#define _STORE_CHK(data) (data).chksum = _L(data) ^ _R(data)

// methods for reading from data
// STATE(read_data_value() is safe
#define STATE(value) ((value) & 3) // 0b11
// INDICATOR(read_data_value() is safe
#define INDICATOR(value) ((value) >> 2)

#define IS_VALID(data) ((data).chksum == (_L(data) ^ _R(data)))

/*
.value
  - state: 2 bit
  - indicator: 14 bit
*/
typedef struct _data_t {
    uint16_t value; // 0xff_ff = 0b1111.1111_1111.1111
    uint8_t chksum;
} data_t;

// 0x20010000 ~ 0x20020000
// 0x20018000
// (0) 1 2 3 4 5 6 (7) [8] 9 A B C D (E) F
data_t data_0 __attribute__((at (0x20010000)));
data_t data_1 __attribute__((at (0x20017000)));
data_t data_2 __attribute__((at (0x2001E000)));

uint32_t warm __attribute__((at (0x20018000)));

void _store_state(data_t *data, uint8_t state)
{
    uint16_t mask = 3; // 0b11
    mask = ~mask;
    data->value = (data->value) & mask;
    data->value += state;
    _STORE_CHK(*data);
}

void _inc_indicator(data_t *data)
{
    data->value += 4; // 0b100
    _STORE_CHK(*data);
}

// ============================================================
// state = indicator = 0
void reset_data(void)
{
    data_0.value = data_0.chksum = 0;
    data_1.value = data_1.chksum = 0;
    data_2.value = data_2.chksum = 0;
}

void store_state(uint8_t state)
{
    _store_state(&data_0, state);
    _store_state(&data_1, state);
    _store_state(&data_2, state);
}

void inc_indicator(void)
{
    _inc_indicator(&data_0);
    _inc_indicator(&data_1);
    _inc_indicator(&data_2);
}

// returns value of data
uint16_t read_data_value(void)
{
    char* ptr = (char*)(&data_0);
    uint8_t available = 1;
    for (int i = 0; i < 3; i++)
    {
        data_t data = *((data_t*)ptr);
        if (IS_VALID(data))
        {
            available = 1;
            break;
        }
        ptr += 0x7000;
    }
    
    if (available)
    {
        data_t data = *((data_t*)ptr);
        // data_0 = data_1 = data_2 = data;
        
        data_0 = data;
        data_1 = data;
        data_2 = data;

        return data.value;
    }
    else
    {
        warm = 0;
        HAL_NVIC_SystemReset();
        // return 0xffff;
    }
}


#endif // DATA_C
