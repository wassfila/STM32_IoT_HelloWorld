#include "mbed.h"

#include "rfmesh.h"
#include "dimm.h"
#include "utils.h"

//------------------------------------- CONFIG -----------------------------------------
const uint8_t CHANNEL = 10;
const uint8_t NODEID = 25;
//--------------------------------------------------------------------------------------

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);

Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
//RFPIO Layout !!!!
RfMesh hsm(&rasp,           PA_5,  PB_12, PB_13, PB_15, PB_14, PA_4);

//                  sync,ch1,2,   3,    4
Dimm dimmer(&rasp,PA_15,PA_8,PA_9,PA_10,PA_11);

DigitalOut relay(PB_3);


void the_ticker()
{
    myled = !myled;
}

//100K => 14.18 ms
void cpu_delay()
{
    volatile uint32_t count = 0;
    for(uint32_t i=0;i<100000;i++)
    {
        count++;
    }
}

void rf_message_received(uint8_t *data,uint8_t size)
{
    rasp.printf("rf>Rx message Handler :");
    //print_tab(&rasp,data,size);
    if(data[0x01] == 0x1B)//pid : light
    {
        uint16_t light_val = data[4];
        light_val <<=8;
        light_val += data[5];
        rasp.printf("light %d\r",light_val);

        dimmer.set_level(0,light_val);
        dimmer.set_level(1,light_val);
        dimmer.set_level(2,light_val);
        dimmer.set_level(3,light_val);

        if(light_val == 0 )
        {
            //we got a flash on switch off as well !!!
            cpu_delay();//wait one cycle so that we're sure all pwm pulses are at 0
            dimmer.syncIrq.disable_irq();//avoid floating jitter
            relay = 1;//off
        }
        else
        {
            //switching on the relay will flash any way even with value of 1 and delaying the start of sync handling
            //switch off, no sync irg, make sure all outputs are low before switching on (set level only prepare for next sync irq)
            //TIM1->CCR1 = 0;            TIM1->CCR2 = 0;            TIM1->CCR3 = 0;            TIM1->CCR4 = 0;
            //cpu_delay();//wait more than 1 pwm period to make sure outputs are at 0
            relay = 0;//switch on
            //should wait here many cycles to avoid light flash
            //probably due to wrong zero detection on start of waves
            //with 90 ms (6 x delay) still a smale wiggle when setting 0x7D0 from 0
            //this flash comes even when setting 0x0001 from 0, spontaneous relay-Dimmer flash
            //~ 90 ms 
            cpu_delay();cpu_delay();cpu_delay();cpu_delay();cpu_delay();cpu_delay();
            
            dimmer.syncIrq.enable_irq();
        }

    }
}

void init()
{
    relay = 1;//Off
    
    uint8_t * p_UID = (uint8_t*) 0x1FFFF7E8;
	
	rasp.printf("Light Dimmer> U_ID: ");
	print_tab(&rasp,p_UID,12);
	rasp.printf("Light Dimmer> Node ID: %d\r",NODEID);

    rasp.printf("Hello Light Dimmer\r");


    tick_call.attach(&the_ticker,1);


    dimmer.init();//here irq is enabled

    hsm.init(CHANNEL);
    rasp.printf("Light Dimmer listening at channel %d\r",CHANNEL);
    hsm.setNodeId(NODEID);
    
    hsm.print_nrf();

    hsm.attach(&rf_message_received,RfMesh::CallbackType::Message);

}

int main() 
{

    init();

    while(1) 
    {
        wait(10);
        rasp.printf("Nb int : %d\r",dimmer.intCount);
        dimmer.intCount = 0;
    }
}
