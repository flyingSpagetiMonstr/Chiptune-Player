#ifndef DATA_C
#define DATA_C

// ################################
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
// ################################

#define WARM_CHK 0x1234ABCD

#define R(data) ((data).value & 0xff)
#define L(data) ((data).value >> 8)

// methods for reading from data
#define STATE(data) ((data).value & 0b11) 
#define INDICATOR(data) ((data).value >> 2)

#define STORE_CHK(data) (data).chksum = L(data) ^ R(data)
#define VERIFY_CHK(data) ((data).chksum == (L(data) ^ R(data)))

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

typedef struct _data_t {
	uint16_t value; // 0xff_ff = 0b1111.1111_1111.1111
	uint8_t chksum;
} data_t;

data_t data_0 __attribute__((at (0x20010000)));
data_t data_1 __attribute__((at (0x20014000)));
data_t data_2 __attribute__((at (0x20018000)));

uint32_t warm __attribute__((at (0x2001C000)));

// methods fro writing/changing data
void store_state(data_t *data, uint8_t state)
{
	data->value = data->value & ~(0b11);
	data->value += state;
	STORE_CHK(*data);
}

void inc_indicator(data_t *data)
{
	data->value += 0b100;
	STORE_CHK(*data);
}

#endif // DATA_C