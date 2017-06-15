#include "mbed.h"

#include "rfmesh.h"

#include "mpu6050.h"

float sum = 0;
uint32_t sumCount = 0;
 
MPU6050 mpu6050(PB_7,PB_6);
//I2C i2c(PB_7,PB_6);

Timer t;

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
DigitalOut timetest(PA_15);
Ticker tick_call;
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
//RFPIO Layout !!!!
//RfMesh mesh(&rasp,           PA_5,  PB_12, PB_13, PB_15, PB_14, PA_4);

void the_ticker()
{
    myled = !myled;
    
}
void rf_message_received(uint8_t *data,uint8_t size)
{
    /*rasp.printf("rf>Rx message Handler : 0x");
    for(int i = 0; i < size; i++)
    {
        rasp.printf(" %02x",data[i]);
    }
    rasp.printf("\r\n");*/
}

void init()
{

    rasp.printf("Hello Inertial Motion Unit simple\n");


    /*tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.nrf.selectChannel(22);
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);

    timetest = 0;
    */

    wait_ms(10);
    // Read the WHO_AM_I register, this is a good test of communication
    uint8_t whoami = mpu6050.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050
    rasp.printf("I AM 0x%x\n\r", whoami);
    rasp.printf("I SHOULD BE 0x68\n\r");

    /*
    wait_ms(10);
    i2c.frequency(100000);  // use fast (400 kHz) I2C  
    wait_ms(10);
  
    char add = 0x68<<1;//not 0x68 but 0xD0
    char subadd = 0x75;
    char data;
    //for(int i=0;i<256;i++)    {        i2c.write(i, &subadd, 1);}
    i2c.write(add, &subadd, 1, 1); // no stop
    i2c.read(add, &data, 1);
    rasp.printf("Who am I is : 0x%x\r\n",data);*/

}

int main() 
{
    init();

    while(1) 
    {
        wait_ms(1000);
    }
}
