#include "mbed.h"

#include "ultrasound.h"

//FlashIAP flash;

Serial   rasp(PB_10, PB_11, 115200);

DigitalOut myled(PC_13);

//Trigger, Echo
ultrasound mySens(PA_8,PA_9);

Ticker tick_call;

int count = 0;

void the_ticker()
{
    myled = !myled;
}

void echo_back_handler(uint32_t dist_cm)
{
    rasp.printf("echo received with pulse width : %d  cm\n",dist_cm);
}

void init()
{

    rasp.printf("Hello Ultrasound\n");
    tick_call.attach(&the_ticker,1);

    mySens.attach(&echo_back_handler);
}



int main() 
{
    init();

    while(1) 
    {
        wait(1.0);
        rasp.printf("Sending Echo\n");
        mySens.send_echo();
    }
}
