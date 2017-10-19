#include "mbed.h"

#include "rfmesh.h"
#include "dimm.h"
#include "utils.h"


Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);

Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
//RFPIO Layout !!!!
RfMesh hsm(&rasp,           PA_5,  PB_12, PB_13, PB_15, PB_14, PA_4);

Dimm dimmer(&rasp,PA_15,PA_8,PA_9,PA_10,PA_11);

void the_ticker()
{
    myled = !myled;
}


void rf_message_received(uint8_t *data,uint8_t size)
{
    rasp.printf("rf>Rx message Handler :");
    print_tab(&rasp,data,size);

    if((data[0] == 5) && (data[1] = 'B') )
    {
    }
}

void init()
{

    rasp.printf("Hello Light Dimmer\n");


    tick_call.attach(&the_ticker,1);

    hsm.init(10);//RF Channel 10
    hsm.attach(&rf_message_received,RfMesh::CallbackType::Message);

}

int main() 
{
    init();

    while(1) 
    {
        wait(1);
    }
}
