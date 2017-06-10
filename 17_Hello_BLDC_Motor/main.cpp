#include "mbed.h"

#include "rfmesh.h"

#include "bldc.h"

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
DigitalOut timetest(PA_15);
Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
//RFPIO Layout !!!!
RfMesh mesh(&rasp,           PA_5,  PB_12, PB_13, PB_15, PB_14, PA_4);

bldc myMotor(&rasp,PA_8,PA_9,PA_10,PA_11);

void the_ticker()
{
    myled = !myled;
    
}
uint16_t delay = 500;
float norm = 0.0;
int16_t a_diff = 0;
bool pos_ctrl = false;

void rf_message_received(uint8_t *data,uint8_t size)
{
    /*rasp.printf("rf>Rx message Handler : 0x");
    for(int i = 0; i < size; i++)
    {
        rasp.printf(" %02x",data[i]);
    }
    rasp.printf("\r\n");*/
    if((data[0] == 5) && (data[1] = 'B') )
    {
        uint8_t id = data[2];
        if(id == 0)
        {
            uint16_t val = data[3];
            val<<=8;
            val |= data[4];
            //0 -> 2000 : 1000 -> 10;
            //if(val == 0) val = 1;
            float vect = val;
            if(vect < 0.1) vect = 0.1;
            vect = 100000 / vect;// 0->1
            if(vect > 500) vect = 500;
            delay = vect;
            rasp.printf("bldc speed : %u : delay = %u\r\n",val, delay);
            pos_ctrl = false;
        }
        if(id == 1)
        {
            uint16_t val = data[3];
            val<<=8;
            val |= data[4];
            float vect = val;// 0 - 1000
            norm = vect/1000;// 0 - 1.0
            rasp.printf("bldc norm : %u\r\n",val);
            pos_ctrl = false;
        }
        if(id == 2)
        {
            int16_t val = data[3];
            val<<=8;
            val |= data[4];
            a_diff += val;// ~ -30 ; +30
            rasp.printf("bldc a_diff : %d\r\n",a_diff);
            pos_ctrl = true;
        }
    }
}

void init()
{

    rasp.printf("Hello BLDC\n");


    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.nrf.selectChannel(22);
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);

    timetest = 0;

}

int main() 
{
    init();

    myMotor.setVector(0,1);
    myMotor.enable = 1;
    int e_angle = 0;
    while(1) 
    {
        if(pos_ctrl)
        {
            if(a_diff > 0)
            {
                e_angle++;
                a_diff--;
            }
            else if(a_diff < 0)
            {
                e_angle--;
                a_diff++;
            }
            else
            {
                //nothing to do maintain pos
            }
            myMotor.setVector(e_angle%256,1.0);
            wait_us(200);
        }
        else
        {
            if(delay == 500)
            {
                timetest = 1;
                myMotor.setVector(e_angle%256,norm);
                timetest = 0;
                wait_ms(100);
            }
            else
            {
                e_angle += 1;
                timetest = 1;
                myMotor.setVector(e_angle%256,norm);
                timetest = 0;
                wait_us(delay);
            }
        }
    }
}
