#include "mbed.h"

#define SENSOR_LIB 1

#ifdef SENSOR_LIB
#include "vl53l0x_class.h"
#endif

/* This VL53L0X Expansion board test application performs a range measurement in polling mode
   on the onboard embedded top sensor. */
 
#define VL53L0_I2C_SDA   PB_7 
#define VL53L0_I2C_SCL   PB_6 

Serial   sp(PB_10, PB_11, 115200);
DigitalOut pintest(PB_13);


#ifdef DIRECT_TEST
I2C i2c(VL53L0_I2C_SDA,VL53L0_I2C_SCL);
#endif

#ifdef SENSOR_LIB
DevI2C device_i2c(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
DigitalOut shutdown(PB_1);
VL53L0X sensor(device_i2c,shutdown,PB_13,DEFAULT_DEVICE_ADDRESS);//Shutdown Not connected PB_1, Int on PB13 - signal
#endif
/*=================================== Main ==================================
=============================================================================*/
int main()
{
    sp.printf("Hello Time Of Flight\n");
    uint32_t distance;
    int  status;
   
#ifdef DIRECT_TEST
    i2c.frequency(400000);
    wait_ms(10);
    pintest = 1;
   char subAddress = 0xC0;
    char data = 0xFF;
    i2c.write(0x52|0x00, &subAddress, 1, 1); // no stop
    i2c.read(0x52|0x01, &data, 1, 0);
    wait_ms(10);
    pintest = 0;
    sp.printf("read from 0xC0 : 0x%x\n",data);
    while(1)
    {
        wait_ms(1000);
    }
#endif

#ifdef SENSOR_LIB
    sp.printf("Init Sensor()\n");
    pintest = 1;
    status = sensor.InitSensor(DEFAULT_DEVICE_ADDRESS);
    pintest = 0;
    if(status)
    {
        sp.printf("Failed to init board! Erro %d\n",status); 
    }
    else
    {
        while(1)
        {
            wait_ms(1000);
            status = sensor.GetDistance(&distance);
            if (status == VL53L0X_ERROR_NONE)
            {
                sp.printf("Distance : %ld\n", distance);
            }
            else
            {
                sp.printf("Measurment Error\n");
            }
        }
    }
#endif

}
