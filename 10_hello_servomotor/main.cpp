#include "mbed.h"
#include "Servo.h"

DigitalOut myled(PC_13);

Serial   rasp(PB_10, PB_11, 115200);

Servo servo0(PA_8);
Servo servo1(PB_0);
Servo servo2(PA_2);
Servo servo3(PA_3);

int main() 
{
    rasp.printf("Hello\n");
    while(1) 
    {
        servo0 = 0;
        servo1 = 0;
        servo2 = 0;
        servo3 = 0;
        myled = 1; // LED is ON
        wait(1.0);

        servo0 = 1;
        servo1 = 1;
        servo2 = 1;
        servo3 = 1;
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
