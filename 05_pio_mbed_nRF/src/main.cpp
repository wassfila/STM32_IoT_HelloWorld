#include "mbed.h"

DigitalOut  myled(PC_13);
Serial      pc_log(PB_10, PB_11, 115200);
Serial      pc_Control(PA_2, PA_3, 115200);

void uart_callback() 
{
    pc_Control.putc(pc_Control.getc());//echo the character
}

int main() 
{
    int count = 0;
    pc_Control.attach(&uart_callback);
    pc_log.printf("Hello from the World of the STM32 Blue pill\r\n");
    while(1) 
    {
        count++;
        pc_log.printf("log cycle:%d\r\n",count);
        myled = 1; // LED is ON
        wait(1.0); // 1 sec
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
