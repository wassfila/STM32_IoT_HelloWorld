#include "mbed.h"

#include "ws2812B.h"

#include "utils.h"

Serial   rasp(PB_10, PB_11, 115200);

DigitalOut myled(PC_13);
Ticker tick_call;

ws2812B rgb_led(PB_13);

void the_ticker()
{
    myled = !myled;
    
}

void init()
{
	uint8_t * p_UID = (uint8_t*) 0x1FFFF7E8;
	
	rasp.printf("stm32_ws2812B> U_ID: ");
	print_tab(&rasp,p_UID,12);

    tick_call.attach(&the_ticker,1);


}


int main() 
{
    init();

    while(true)
    {
        rgb_led.set(0xFF,0xFF,0xFF);
        wait(2);
        rgb_led.set(0x00,0x00,0x00);
        wait(2);
        rgb_led.set(0xFF,0x00,0x00);
        wait(2);
        rgb_led.set(0x00,0xFF,0x00);
        wait(2);
        rgb_led.set(0x00,0x00,0xFF);
        wait(2);
        rgb_led.set(0xFF,0x00,0xFF);
        wait(2);
        rgb_led.set(0x00,0xFF,0x00);
        wait(2);
    }

}
