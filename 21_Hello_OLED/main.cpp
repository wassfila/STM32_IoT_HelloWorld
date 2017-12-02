#include "mbed.h"

#include "rfmesh.h"

#include "SSD1308.h"

#define D_SCL                  PB_6
#define D_SDA                  PB_7
I2C i2c(D_SDA, D_SCL);

//SSD1308 oled(i2c, SSD1308_SA0);
SSD1308 oled(i2c, 0x3C);

Serial   rasp(PB_10, PB_11, 115200);
DigitalOut myled(PC_13);
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
    rasp.printf("rf>Rx message Handler : 0x");
    for(int i = 0; i < size; i++)
    {
        rasp.printf(" %02x",data[i]);
    }
    rasp.printf("\r\n");
}

void init()
{

    rasp.printf("Hello LCD\n");

    tick_call.attach(&the_ticker,1);

    mesh.init();//left to the user for more flexibility on memory management
    mesh.attach(&rf_message_received,RfMesh::CallbackType::Message);

    i2c.frequency(100000);

}

void simple_OLED() 
{
  rasp.printf("OLED test start\r"); 
  rasp.printf("Hello World from ST32F103RB\n\r");    
 
  
  oled.writeString(0, 0, "Hello World !");  
  rasp.printf("Printed something\r");    
  wait(3);
    
  oled.writeString(1, 0, "baz quux");  
  rasp.printf("Printed something\r");  
  wait(3);
    
  oled.fillDisplay(0xAA);
  rasp.printf("Display filled\r");      
  wait(3);
 
  oled.setDisplayOff();
  rasp.printf("Display off\r");          
  wait(0.5);
 
  oled.setDisplayOn();
  rasp.printf("Display on\r");          
  wait(0.5);  
 
  oled.clearDisplay();
  rasp.printf("Display cleared\r");        
  wait(0.5);
 
  oled.writeString(0, 0, "Bye World !");  
  rasp.printf("Printed something\r");    
  wait(3);
 
  rasp.printf("OLED test done\r");  
}

int main() 
{
    init();


    while(1) 
    {
        simple_OLED();
        wait(1);
    }
}
