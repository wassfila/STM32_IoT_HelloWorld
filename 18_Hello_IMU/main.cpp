#include "mbed.h"

#include "rfmesh.h"

#include "mpu6050.h"

float sum = 0;
uint32_t sumCount = 0;
 
MPU6050 mpu6050(PB_7,PB_6);
Timer t;

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
DigitalOut timetest(PA_15);
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
    /*rasp.printf("rf>Rx message Handler : 0x");
    for(int i = 0; i < size; i++)
    {
        rasp.printf(" %02x",data[i]);
    }
    rasp.printf("\r\n");*/
}

void init()
{

    rasp.printf("Hello Inertial Motion Unit\n");


    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.nrf.selectChannel(22);
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);

    timetest = 0;

    // Read the WHO_AM_I register, this is a good test of communication
    wait_ms(10);
    uint8_t whoami = mpu6050.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050
    rasp.printf("I AM 0x%x\n\r", whoami);
    rasp.printf("I SHOULD BE 0x68\n\r");
  
    if (whoami == 0x68) // WHO_AM_I should always be 0x68
    {  
        rasp.printf("MPU6050 is online...");
        wait(1);

        mpu6050.MPU6050SelfTest(mpu6050.SelfTest); // Start by performing self test and reporting values
        rasp.printf("x-axis self test: acceleration trim within : "); rasp.printf("%f", mpu6050.SelfTest[0]); rasp.printf("% of factory value \n\r");
        rasp.printf("y-axis self test: acceleration trim within : "); rasp.printf("%f", mpu6050.SelfTest[1]); rasp.printf("% of factory value \n\r");
        rasp.printf("z-axis self test: acceleration trim within : "); rasp.printf("%f", mpu6050.SelfTest[2]); rasp.printf("% of factory value \n\r");
        rasp.printf("x-axis self test: gyration trim within : "); rasp.printf("%f", mpu6050.SelfTest[3]); rasp.printf("% of factory value \n\r");
        rasp.printf("y-axis self test: gyration trim within : "); rasp.printf("%f", mpu6050.SelfTest[4]); rasp.printf("% of factory value \n\r");
        rasp.printf("z-axis self test: gyration trim within : "); rasp.printf("%f", mpu6050.SelfTest[5]); rasp.printf("% of factory value \n\r");
        wait(1);

        if(mpu6050.SelfTest[0] < 1.0f && mpu6050.SelfTest[1] < 1.0f && mpu6050.SelfTest[2] < 1.0f && mpu6050.SelfTest[3] < 1.0f && mpu6050.SelfTest[4] < 1.0f && mpu6050.SelfTest[5] < 1.0f) 
        {
            mpu6050.resetMPU6050(); // Reset registers to default in preparation for device calibration
            mpu6050.calibrateMPU6050(mpu6050.gyroBias, mpu6050.accelBias); // Calibrate gyro and accelerometers, load biases in bias registers  
            mpu6050.initMPU6050(); rasp.printf("MPU6050 initialized for active data mode....\n\r"); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
            wait(2);
        }
        else
        {
            rasp.printf("Device did not the pass self-test!\n\r");
        }
    }
    else
    {
        rasp.printf("Could not connect to MPU6050: \n\r");
        rasp.printf("%#x \n",  whoami);
    }

}

int main() 
{
    init();

    while(1) 
    {
        //wait_ms(1000);
        // If data ready bit set, all data registers have new data
        if(mpu6050.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) 
        {  // check if data ready interrupt
            mpu6050.readAccelData(mpu6050.accelCount);  // Read the x/y/z adc values
            rasp.printf("0x%X 0x%X 0x%X\r\n", mpu6050.accelCount[0],mpu6050.accelCount[1],mpu6050.accelCount[2]); 
        }

    }
}
