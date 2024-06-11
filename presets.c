#ifndef PRESET_C
#define PRESET_C

#define WIDE
#ifdef WIDE
// enum notes {
//     O_ = 0,
//     C2, CD2, D2, DE2, E2, F2, FG2, G2, GA2, A2, AB2, B2,
//     C3, CD3, D3, DE3, E3, F3, FG3, G3, GA3, A3, AB3, B3,
//     C4 /*mid C*/, CD4, D4, DE4, E4, F4, FG4, G4, GA4, A4 /*440 Hz*/, AB4, B4,
//     C5, CD5, D5, DE5, E5, F5, FG5, G5, GA5, A5, AB5, B5,
// };

const uint32_t periods[] = {0x7fffffff, 15289, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012};

#else
enum notes {
    O_,
    C4 = 1 /*mid C*/, CD4, D4, DE4, E4, F4, FG4, G4, GA4, A4 /*440 Hz*/, AB4, B4,
    C5, CD5, D5, DE5, E5, F5, FG5, G5, GA5, A5, AB5, B5,
};
const uint32_t periods[] = {0x7fffffff, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012};
#endif

typedef struct _sound
{
    // enum notes note;
    int note;
    double duration;
} sound;


#endif // PRESET_C
