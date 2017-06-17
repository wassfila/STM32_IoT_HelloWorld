#include "mbed.h"

#include "VL53L0X.h"

Serial   sp(PB_10, PB_11, 115200);
DigitalOut pintest(PB_13);
I2C i2c(PB_7,PB_6);//SDA, SCL
Timer t;
VL53L0X sensor(&i2c,&t);

int main()
{
    sp.printf("Hello Time Of Flight\n");
    uint32_t distance;
    int  status;
   
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

}
