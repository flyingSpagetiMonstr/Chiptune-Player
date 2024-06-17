#ifndef SCORES_C
#define SCORES_C

#include "presets.c"

#define _BWV846
#define IEVAN_POLKKA

// =========================================================

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


// =========================================================

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

// =========================================================
#ifdef IEVAN_POLKKA

sound Ievan_polkka[] = {
    {46,  400000}, {46,  400000}, {44,  400000}, {42,  200000}, {42,  200000}, {41,  400000}, {37,  200000}, {37,  600000}, {41,  400000},
    {44,  400000}, {44,  200000}, {44,  200000}, {42,  400000}, {41,  400000}, {42,  400000}, {38,  400000}, {38,  400000}, {42,  400000},
    {46,  200000}, {46,  200000}, {46,  200000}, {46,  200000}, {44,  400000}, {42,  400000}, {41,  400000}, {37,  200000}, {37,  400000}, {37,  200000}, {41,  400000},
    {44,  200000}, {44,  200000}, {44,  400000}, {42,  400000}, {41,  200000}, {41,  200000}, {42,  400000}, {39,  200000}, {39,  800000}, { 0,  200000},
    {34,  400000}, {39,  400000}, {39,  600000}, {41,  200000}, {42,  200000}, {42,  200000}, {39,  200000}, {39,  200000}, {39,  400000}, {42,  200000}, {42,  200000},
    {41,  400000}, {37,  400000}, {37,  400000}, {41,  400000}, {42,  400000}, {39,  400000}, {39,  400000}, {39,  200000}, {39,  200000},
    {34,  400000}, {39,  400000}, {39,  600000}, {41,  200000}, {42,  200000}, { 0,  200000}, {39,  400000}, {39,  200000}, {39,  200000}, {39,  200000}, {42,  200000},
    {46,  200000}, {46,  200000}, {46,  200000}, {44,  200000}, {42,  200000}, { 0,  200000}, {41,  400000}, {42,  400000}, {39,  200000}, { 0,  200000}, {39,  400000}, {39,  200000}, {39,  200000},
    {46,  400000}, {46,  400000}, {44,  400000}, {42,  400000}, {41,  400000}, {37,  200000}, {37,  400000}, {37,  200000}, {37,  200000}, {41,  200000},
    {44,  200000}, {44,  200000}, {44,  200000}, {44,  200000}, {42,  200000}, {42,  200000}, {41,  200000}, {41,  200000}, {42,  400000}, {39,  200000}, {39,  800000}, {39,  200000},
    {46,  400000}, {46,  400000}, {44,  400000}, {42,  400000}, {41,  400000}, {37,  400000}, {37,  200000}, {37,  200000}, {37,  200000}, {41,  200000},
    {44,  200000}, {44,  200000}, {44,  200000}, {44,  200000}, {42,  400000}, {41,  400000}, {42,  400000}, {39,  200000}, {39, 1000000},
    {00, 2000000}
};

uint16_t const Ievan_polkka_len = sizeof(Ievan_polkka)/sizeof(sound);

typedef sound *score_t;

score_t scores[3] = {anthem, BWV846, Ievan_polkka};
uint16_t lens[3] = {anthem_len, BWV846_len, Ievan_polkka_len};

#endif


#endif // SCORES_C


