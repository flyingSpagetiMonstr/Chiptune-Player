// order

enum states {
    WAIT, PLAY1, PLAY2, PLAY3 
};
/*
 -> WAIT (for input)
 case 0: no effect
 case 1: PLAY1 -> WAIT
 case 2: PLAY2 -> WAIT
 case 3: PLAY3 -> WAIT
*/


typedef struct _data_t {
	uint8_t value;
	// uint8_t chksum;
} data_t;
// chksum = ~value

data_t state_0 __attribute__((at (0x20010000)));
data_t state_1 __attribute__((at (0x20014000)));
data_t state_2 __attribute__((at (0x20018000)));

uint32_t marker __attribute__((at (0x2001a000)));

int data_chk()
{
	
}