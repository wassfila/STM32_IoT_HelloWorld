#include "mbed.h"
#include "Servo.h"

//FlashIAP flash;

Serial   rasp(PB_10, PB_11, 115200);

DigitalOut myled(PC_13);

Servo servo(PA_11);
uint8_t s_id;
int16_t val;
uint8_t update = 0;

uint16_t crc_Fletcher16( uint8_t const *data, uint8_t count )
{
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	int index;

	for( index = 0; index < count; ++index )
	{
		sum1 = (sum1 + data[index]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}

	return (sum2 << 8) | sum1;
}

// size(size+data) : data : crc
bool crc_check(uint8_t const *data)
{
	bool result = 1;
	uint8_t size = data[0];
	uint16_t crc = crc_Fletcher16(data,size);//check the data without excluding the crc
	if( (data[size] != (crc >> 8) ) || (data[size+1] != (crc & 0xFF) ) )
	{
		result = 0;
	}
	return result;
}

void process_uart(uint8_t *buf)
{
    if(crc_check(buf))
    {
        if(buf[1] == 'S')
        {
            s_id = buf[2];
            val = buf[3];
            val <<=8;
            val += buf[4];

            float spos = val;
            spos /= 10000;
            servo = spos;
            update = 1;
        }
    }
    else
    {
        rasp.printf("Processing UART CRC Fail\n");
        rasp.printf("0x %x %x %x %x %x %x %x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
    }
}

uint8_t ubuf[32];
uint8_t bufi = 0;
uint8_t ud_size = 0;
uint8_t is_getting_late = 0;
Ticker uart_tick;

void uart_ticker()
{
    if(is_getting_late)
    {
        if(bufi != 0)
        {
            rasp.printf("UART reset counter got (%d)\n",bufi);
            rasp.printf("0x");
            for(int i=0;i<bufi;i++)
            {
                rasp.printf(" %02x",ubuf[i]);
            }
            rasp.printf("\n");
            bufi = 0;
        }
    }
    else
    {
        is_getting_late = 1;
    }
    if(update)
    {
        rasp.printf("Servo %d at %d\n",s_id,val);
        update = 0;
    }
}

void uart_callback() 
{
    char c = rasp.getc();
    if(bufi < 32)
    {
        ubuf[bufi] = c;
        if(bufi == 0)
        {
            ud_size = ubuf[0];
            bufi++;
        }
        else if(bufi == ud_size+1)//size + 2x CRC
        {
            process_uart(ubuf);
            bufi = 0;
        }
        else
        {
            bufi++;
        }
    }
    else//ovewrite last char
    {
        bufi = 0;
    }
    is_getting_late = 0;
}

void init()
{
    //printf(id);
    rasp.attach(&uart_callback);
    uart_tick.attach(&uart_ticker,0.1);
    rasp.printf("Hello Servo from STM32\n");
    servo = 0;
    myled = 1; // LED is ON
    wait(1.0);

    servo = 1;
    myled = 0; // LED is OFF
    wait(1.0); // 1 sec
}



int main() 
{
    int count = 0;
    init();

    while(1) 
    {
        count++;
        rasp.printf("log cycle:%d\n",count);

        myled = 1; // LED is ON
        wait(1.0);

        myled = 0; // LED is OFF
        wait(1.0); // 1 sec


    }
}
