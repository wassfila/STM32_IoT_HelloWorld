#include "mbed.h"

DigitalOut myled(PC_13);


int main() 
{
    while(1) 
    {
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
