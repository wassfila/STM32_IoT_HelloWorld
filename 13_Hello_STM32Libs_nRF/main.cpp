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
    static int count = 0;
    rasp.printf("tick log cycle:%d - ",count++);
    myled = !myled;
    mesh.nrf_print_status();
}

#define TRANSFER_SIZE   32

char rxData[TRANSFER_SIZE];
int rxDataCnt = 0;

void rf_message_received()
{
    rasp.printf("rf>>> I got a message\n");
    /*if ( mesh.nrf.readable() ) 
    {
        mesh.nrf_print_status();
        rxDataCnt = mesh.nrf.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );

        // Display the receive buffer contents via the host serial link
        rasp.printf("Rx message Handler : 0x");
        for ( int i = 0; rxDataCnt > 0; rxDataCnt--, i++ ) {
            //pc.putc( rxData[i] );
            rasp.printf(" %x",rxData[i]);
        }
        rasp.printf("\r\n");

    }*/
}

void init()
{
    rasp.printf("Hello STM32 Libs nRF\n");

    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.attach(&rf_message_received,RfMesh::Message);
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
        /*
        if ( mesh.nrf.readable() ) 
        {
            nrf_print_status();
            rxDataCnt = mesh.nrf.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );

            // Display the receive buffer contents via the host serial link
            rasp.printf("main Rx : 0x");
            for ( int i = 0; rxDataCnt > 0; rxDataCnt--, i++ ) {
                //pc.putc( rxData[i] );
                rasp.printf(" %x",rxData[i]);
            }
            rasp.printf("\r\n");

        }*/
    }
}
