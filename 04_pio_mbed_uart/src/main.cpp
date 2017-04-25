#include "mbed.h"

DigitalOut  myled(PC_13);
Serial      pc(PB_10, PB_11, 115200);


int main() 
{
    int count = 0;
    pc.printf("Hello from the World of the STM32 Blue pill\r\n");
    while(1) 
    {
        count++;
        pc.printf("log cycle:%d\r\n",count);
        myled = 1; // LED is ON
        wait(1.0); // 1 sec
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
