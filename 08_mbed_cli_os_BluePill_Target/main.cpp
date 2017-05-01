#include "mbed.h"
  
int main() {
    
    Serial      pc(PA_2, PA_3);
    DigitalOut  myled(LED1);
    
    while(1) 
    {
        // The on-board LED is connected, via a resistor, to +3.3V (not to GND). 
        // So to turn the LED on or off we have to set it to 0 or 1 respectively
        myled = 0;      // turn the LED on
        wait_ms(1000);   // 200 millisecond
        myled = 1;      // turn the LED off
        wait_ms(1000);  // 1000 millisecond
        pc.printf("Blink\r\n");
    }
}
