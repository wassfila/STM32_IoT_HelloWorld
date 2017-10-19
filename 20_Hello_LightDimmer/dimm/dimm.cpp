
#include "dimm.h"

static Dimm* handler = 0;

uint32_t latches[4];

void sync_irq()
{
    handler->intCount++;
}

Dimm::Dimm(Serial *ps,PinName Sync, PinName ch1, PinName ch2, PinName ch3, PinName ch4):
                pser(ps),
                syncIrq(Sync),
                pwm1(ch1),
                pwm2(ch2),
                pwm3(ch2),
                pwm4(ch3)
{
    intCount = 0;

    syncIrq.fall(&sync_irq);
    syncIrq.mode(PullNone);
    syncIrq.enable_irq();

    handler = this;

    pwm1 = 0;
    pwm2 = 0;
    pwm3 = 0;
    pwm4 = 0;

    latches[0] = 0;
    latches[1] = 0;
    latches[2] = 0;
    latches[3] = 0;

    
}

void Dimm::init()
{

}

void Dimm::set_level(uint8_t channel,uint16_t value)
{
    if(value < level::min)
    {
        value = level::min;
    }
    if(value > level::tim_per)
    {
        value = level::tim_per;
    }
    uint16_t delay = level::tim_per - (value - level::min);
    if(channel >= 3)
    {
        latches[channel] = delay;
    }
}
