#include "mbed.h"

#include "rfmesh.h"

//FlashIAP flash;

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
RfMesh mesh(&rasp,           PC_15, PA_4, PA_5,   PA_7,  PA_6,    PA_0);

void the_ticker()
{
    //static int count = 0;
    //rasp.printf("tick log cycle:%d\n",count++);
    myled = !myled;
    //mesh.nrf.print_status();
}

#define TRANSFER_SIZE   32

char rxData[TRANSFER_SIZE];
int rxDataCnt = 0;

void rf_message_received(uint8_t *data,uint8_t size)
{
    rasp.printf("rf>Rx message Handler : 0x");
    for(int i = 0; i < size; i++)
    {
        rasp.printf(" %02x",data[i]);
    }
    rasp.printf("\r\n");
}

void init()
{
    rasp.printf("Hello STM32 Libs nRF\n");

    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);
    //mesh.print_nrf();

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
