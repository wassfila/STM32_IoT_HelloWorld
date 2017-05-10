#include "mbed.h"
#include "Servo.h"

DigitalOut myled(PC_13);

class ServoOut: public PwmOut
{
    public:
        ServoOut(PinName pin) : PwmOut(pin)
        {
            PwmOut::period_ms(20);
            offset = 1.5;
            period = 20;
            range = 1;//+- 1 ms
        }
    public:
        void setpos(float pos)
        {
            float pos_us = ((offset + pos * range) * 1000);
            PwmOut::pulsewidth_us(pos_us);
        }
    public:
        float period;
        float offset;
        float range;
};

ServoOut servo(PB_4);

Servo servo2(PA_11);

int main() 
{
    while(1) 
    {
        servo.setpos(1);
        servo2 = 0;
        //servo.pulsewidth_us(500);
        myled = 1; // LED is ON
        wait(1.0);
        servo.setpos(-1);
        servo2 = 1;
        //servo.pulsewidth_us(2500);
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
