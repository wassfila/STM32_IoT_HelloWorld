#include "mbed.h"

#include "glibr.h"

//RFNode pinout
glibr gsensor(PB_7,PB_6);

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
Ticker tick_call;

void the_ticker()
{
    myled = !myled;
    
}

void init()
{

    rasp.printf("Hello APDS9960\n");

    tick_call.attach(&the_ticker,1);

    rasp.printf("before init Mode is : %u\n",gsensor.getMode());

    if ( gsensor.ginit() ) {
        rasp.printf("APDS-9960 initialization complete\n\r");
    } else {
        rasp.printf("Something went wrong during APDS-9960 init\n\r");
    }
 
    rasp.printf("after init Mode is : %u\n",gsensor.getMode());

}

void init_gesture()
{
    // Start running the APDS-9960 gesture sensor engine
    if ( gsensor.enableGestureSensor(false) ) {
        rasp.printf("Gesture sensor is now running\n\r");
        rasp.printf("Mode is : %u\n",gsensor.getMode());
    } else {
        rasp.printf("Something went wrong during gesture sensor init!\n\r");
    }
}

void poll_gesture()
{
    while(1) 
    {
        if ( gsensor.isGestureAvailable() ) {
            switch ( gsensor.readGesture() ) {
                case DIR_UP:
                    rasp.printf("UP\n");
                    break;
                case DIR_DOWN:
                    rasp.printf("DOWN\n");
                    break;
                case DIR_LEFT:
                    rasp.printf("LEFT\n");
                    break;
                case DIR_RIGHT:
                    rasp.printf("RIGHT\n");
                case DIR_NEAR:
                    rasp.printf("NEAR\n");
                    break;
                case DIR_FAR:
                    rasp.printf("FAR\n");
                    break;
                default:
                    rasp.printf("NONE\n");
            }
        }
        wait(0.010);
    }

}

void init_light()
{
    
    if ( gsensor.enableLightSensor() ) 
    {
        rasp.printf("Light sensor is on\n\r");
    } else 
    {
        rasp.printf("Something went wrong during light sensor init!\n\r");
    }
}

void poll_light()
{
    while(true)
    {
        uint16_t val,R,G,B;
        gsensor.readAmbientLight(val);
        gsensor.readRedLight(R);
        gsensor.readGreenLight(G);
        gsensor.readBlueLight(B);
        rasp.printf("Light : %d ; R:%d, G:%d, B:%d\r\n",val,R,G,B);
        wait(1);
    }
}

int main() 
{
    init();

    //init_gesture();
    //poll_gesture();

    //init_light();
    //poll_light();

    bool res = gsensor.enableProximitySensor();
    rasp.printf("enableProximitySensor: %u\n",res);
    uint8_t value = 0;
    while(true)
    {
        uint8_t val;
        gsensor.readProximity(val);
        if(val > 30)
        {
            value = val;
            rasp.printf("set value(%u)\n",value);
        }
        wait(0.1);
    }

}
