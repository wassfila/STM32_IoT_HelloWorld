#include "mbed.h"


Serial   sp(PB_10, PB_11, 115200);
DigitalOut pintest(PB_13);

#define MBEDOS_READYLIB 1

#ifdef MY_LIB_REMAKE
    #include "vl53l0x.h"
    vl53l0x sensor(&sp,PB_7,PB_6);//SDA, SCL
#endif

#ifdef MBEDOS_READYLIB
#include "VL53L0X.h"
I2C i2c(PB_7,PB_6);//SDA, SCL
Timer t;
VL53L0X sensor(&i2c,&t);
#endif

int main()
{
    sp.printf("Hello Time Of Flight\n");
    uint32_t distance;
    int  status;
   
#ifdef MBEDOS_READYLIB
    sp.printf("Init Sensor()\n");
    pintest = 1;
    status = sensor.init();
    pintest = 0;

    while(true)
    {
        wait_ms(1000);
        uint16_t distance_mm = sensor.readRangeSingleMillimeters();
        sp.printf("distance is : %u\n",distance_mm);
    }
#endif


#ifdef MY_LIB_REMAKE
    pintest = 1;
    if(sensor.isPresent() )
    {
        pintest = 0;
        sp.printf("ToF> I'm here\n");
        sensor.init();
    }
    pintest = 0;
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
