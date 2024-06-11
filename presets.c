#ifndef PRESET_C
#define PRESET_C

// ################################
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
// ################################

const uint16_t periods[] = {0, 15289, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012};

typedef struct _sound
{
    uint8_t note;
    uint32_t duration;
} sound;

#endif // PRESET_C
