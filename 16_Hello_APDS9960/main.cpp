#include "mbed.h"

#include "rfmesh.h"
#include "glibr.h"

//RFNode pinout
glibr GSensor(PB_7,PB_6);

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
//RFPIO Layout !!!!
RfMesh mesh(&rasp,           PA_5,  PB_12, PB_13, PB_15, PB_14, PA_4);

void the_ticker()
{
    myled = !myled;
    
}

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

    rasp.printf("Hello APDS9960\n");

    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);

    if ( GSensor.ginit() ) {
        rasp.printf("APDS-9960 initialization complete\n\r");
    } else {
        rasp.printf("Something went wrong during APDS-9960 init\n\r");
    }
 
    // Start running the APDS-9960 gesture sensor engine
    if ( GSensor.enableGestureSensor(true) ) {
        rasp.printf("Gesture sensor is now running\n\r");
    } else {
        rasp.printf("Something went wrong during gesture sensor init!\n\r");
    }


}

int main() 
{
    init();

    while(1) 
    {
        if ( GSensor.isGestureAvailable() ) {
            switch ( GSensor.readGesture() ) {
                case DIR_UP:
                    rasp.printf("UP\n");
                    break;
                case DIR_DOWN:
                    rasp.printf("DOWN\n");
                    break;
                case DIR_LEFT:
                    rasp.printf("LEFT\n");
                    break;
                case DIR_RIGHT:
                    rasp.printf("RIGHT\n");
                case DIR_NEAR:
                    rasp.printf("NEAR\n");
                    break;
                case DIR_FAR:
                    rasp.printf("FAR\n");
                    break;
                default:
                    rasp.printf("NONE\n");
            }
        }
        wait(0.010);
    }
}
