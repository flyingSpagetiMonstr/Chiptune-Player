#ifndef DATA_C
#define DATA_C

// ################################
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
// ################################

// #define WARM_MARK 0x1234ABCD
#define WARM_MARK 0xC0DECAFE

#define _R(data) ((data).value & 0xff)
#define _L(data) ((data).value >> 8)
#define _STORE_CHK(data) (data).chksum = _L(data) ^ _R(data)

// methods for reading from data
// STATE(read_data() is safe
#define STATE(data) ((data).value & 0b11) 
// INDICATOR(read_data() is safe
#define INDICATOR(data) ((data).value >> 2)

#define IS_VALID(data) ((data).chksum == (_L(data) ^ _R(data)))

/*
 -> WAIT (for input)
 case 0: no effect
 case 1: PLAY1 -> WAIT
 case 2: PLAY2 -> WAIT
 case 3: PLAY3 -> WAIT
*/
#define WAIT 0
#define PLAY1 1
#define PLAY2 2
#define PLAY3 3

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
// (0) 1 2 3 4 5 6 (7) [8] 9 A B C D E (F)
data_t data_0 __attribute__((at (0x20010000)));
data_t data_1 __attribute__((at (0x20017000)));
data_t data_2 __attribute__((at (0x2001F000)));

uint32_t warm __attribute__((at (0x20018000)));

// state = indicator = 0
void init_data(void)
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

data_t read_data(void)
{
	uint8_t ptr = &data_0;
	uint8_t available = 0;
	for (int i = 0; i < 3; i++)
	{
		data_t data = *((data_t*)ptr);
		if (IS_VALID(data))
		{
			available = 1;
			break;
		}
		ptr += 4000;
	}
	
	if (available)
	{
		data_t data = *((data_t*)ptr);
		data_0 = data_1 = data_2 = data;
		return data;
	}
	else
	{
		warm = 0;
		HAL_NVIC_SystemReset();
	}
}

void _store_state(data_t *data, uint8_t state)
{
	data->value = data->value & ~(0b11);
	data->value += state;
	_STORE_CHK(*data);
}

void _inc_indicator(data_t *data)
{
	data->value += 0b100;
	_STORE_CHK(*data);
}

#endif // DATA_C