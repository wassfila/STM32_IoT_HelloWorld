
#ifndef __DIMM__
#define __DIMM__

#include "mbed.h"

namespace level {
    const uint16_t min = 185;
    const uint16_t tim_per = 9900;
}

class Dimm
{
    

    public:
        Dimm(Serial *ps,PinName Sync, PinName ch1, PinName ch2, PinName ch3, PinName ch4);
        void init();
        void set_level(uint8_t channel,uint16_t value);
    public:
        PwmOut  pwm1;
        PwmOut  pwm2;
        PwmOut  pwm3;
        PwmOut  pwm4;
    public:
        Serial      *pser;
        InterruptIn syncIrq;
        uint32_t    intCount;
};

#endif /*__DIMM__*/
