#include "mbed.h"

DigitalOut myled(PC_13);

PwmOut my_pwm(PB_4);

int main() 
{
    my_pwm.period_ms(20);
    while(1) 
    {
        my_pwm.write(0.5);
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        my_pwm.write(0.0);
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
