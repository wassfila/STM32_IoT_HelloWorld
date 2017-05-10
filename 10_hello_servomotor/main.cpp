#include "mbed.h"
#include "Servo.h"

DigitalOut myled(PC_13);

Servo servo(PA_11);

int main() 
{
    while(1) 
    {
        servo = 0;
        myled = 1; // LED is ON
        wait(1.0);

        servo = 1;
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
