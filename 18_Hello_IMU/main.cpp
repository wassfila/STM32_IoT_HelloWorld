#include "mbed.h"

#include "mpu6050.h"

DigitalOut pintest(PB_13);
Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;
 
MPU6050 mpu6050(&rasp,PB_7,PB_6);
bool isMPU_Ready = false;
Timer t;


int16_t accelVals[3];  // Stores the 16-bit signed accelerometer sensor output
int16_t gyroVals[3];   // Stores the 16-bit signed gyro sensor output
bool inUpdate = false;
void the_ticker()
{
    myled = !myled;
    if(!inUpdate && isMPU_Ready)
    {
        rasp.printf("Accel : %d %d %d ; Gyro : %d %d %d\n", accelVals[0],accelVals[1],accelVals[2],gyroVals[0],gyroVals[1],gyroVals[2]); 
    }
}

void init()
{
    rasp.printf("Hello Inertial Motion Unit\n");
    tick_call.attach(&the_ticker,1);

    // Read the WHO_AM_I register, this is a good test of communication
    if(mpu6050.isPresent())
    {
        mpu6050.startup();
        isMPU_Ready = true;
    }
}

int main() 
{
    init();

    while(1) 
    {
        // If data ready bit set, all data registers have new data
        if(isMPU_Ready)
        {
            if(mpu6050.readByte(INT_STATUS) & 0x01) 
            {  // check if data ready interrupt
                inUpdate = true;// lock to avoid partial data update
                mpu6050.readAccelData(accelVals);  // Read the x/y/z adc values ~ 300 us
                mpu6050.readGyroData(gyroVals);
                inUpdate = false;
            }
            wait_ms(10);//give some fresh air for low prio ticker to run without missing cycles
        }
        else
        {
            wait_ms(1000);
        }
    }
}
