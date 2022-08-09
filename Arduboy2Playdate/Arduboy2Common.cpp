#include "Arduboy2Common.h"

void randomSeed(unsigned int dwSeed)
{
    srandom(dwSeed);
}

long random(long howbig)
{
    if (howbig == 0) {
        return 0;
    }

    return random() % howbig;
}

long random(long howsmall, long howbig)
{
    if (howsmall >= howbig) {
        return howsmall;
    }

    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

char *ltoa(long value, char *string, int radix)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v;
    int sign;
    char *sp;

    if (string == NULL) {
        return 0 ;
    }

    if (radix > 36 || radix <= 1) {
        return 0 ;
    }

    sign = (radix == 10 && value < 0);
    if (sign) {
        v = -value;
    } else {
        v = (unsigned long)value;
    }

    while (v || tp == tmp) {
        i = v % radix;
        v = v / radix;
        if (i < 10) {
            *tp++ = i + '0';
        } else {
            *tp++ = i + 'a' - 10;
        }
    }

    sp = string;

    if (sign) {
        *sp++ = '-';
    }
    while (tp > tmp) {
        *sp++ = *--tp;
    }
    *sp = 0;

    return string;
}
char *itoa(int value, char *string, int radix)
{
    return ltoa(value, string, radix) ;
}


unsigned long millis()
{
    return pd->system->getCurrentTimeMilliseconds();
}

void delay(int ms)
{
    // TODO: implement me.
}

PlaydateAPI *pd;

int update(__attribute__ ((unused)) void* ud)
{
    loop();
    return 1;
}

extern "C" int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg) {
    if (event == kEventInit) {
        pd = playdate;
        setup();
        pd->system->setUpdateCallback(update, NULL);
    }
    return 0;
}