#include "mbed.h"

#include "rfmesh.h"

//FlashIAP flash;

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;
RfMesh mesh(&rasp);

void the_ticker()
{
    static int count = 0;
    rasp.printf("tick log cycle:%d\n",count++);
    myled = !myled;
}

void init()
{
    rasp.printf("Hello STM32 Libs nRF\n");

    tick_call.attach(&the_ticker,1);

    mesh.init();
    mesh.print_nrf();

    myled = 1; // LED is ON
    wait(1.0);

    myled = 0; // LED is OFF
    wait(1.0); // 1 sec

}

int main() 
{
    init();

    while(1) 
    {
        wait(1.0);
    }
}
