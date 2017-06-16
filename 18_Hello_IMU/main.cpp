#include "mbed.h"

#include "rfmesh.h"

#include "mpu6050.h"

DigitalOut pintest(PB_13);
Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;
 
MPU6050 mpu6050(&rasp,PB_7,PB_6);
bool isMPU_Ready = false;
Timer t;


#define USE_RF_MESH 0
//nRF Modules 1:Gnd, 2:3.3v, 3:ce,  4:csn, 5:sck, 6:mosi, 7:miso, 8:irq 
#if BOARD_IS_RF_DONGLE ==1
    #if USE_RF_MESH ==1
    RfMesh mesh(&rasp,           PC_15, PA_4, PA_5,   PA_7,  PA_6,    PA_0);
    #endif
#endif

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
    rasp.printf("Hello Inertial Motion Unit\n");
    tick_call.attach(&the_ticker,1);

#if USE_RF_MESH == 1
    mesh.init();//left to the user for more flexibility on memory management
    mesh.nrf.selectChannel(22);
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);
#endif

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
