#include "mbed.h"


//FlashIAP flash;

Serial   rasp(PB_10, PB_11, 115200);

DigitalOut myled(PC_13);

//Trigger, Echo, update, timeout, callback
DigitalOut trigger(PA_8);
DigitalIn echo(PA_9);
DigitalOut uart_irqSignal(PA_10);

Ticker tick_call;
Ticker sonar;

int count = 0;

void the_ticker()
{
    count++;
    rasp.printf("tick sonar log cycle:%d\n",count);
    myled = !myled;

}

void the_sonar()
{
    trigger = 1;
    wait_us(10);
    trigger = 0;
}

class Ultrasound
{
    public:
        Ultrasound(PinName trigger, PinName echo):trig_pin(trigger),echo_pin(echo)
        {

        }
    private:
        DigitalOut trig_pin;
        InterruptIn echo_pin;
};

void serial_int()
{
    USART3->CR1 &= ~USART_CR1_TXEIE_Msk;
    uart_irqSignal = 1;
    wait_us(30);
    uart_irqSignal = 0;
}

void init()
{

    //(USART3_BASE)->CR1 |= ((__INTERRUPT__) & UART_IT_MASK))
    USART3->CR1 |= USART_CR1_TXEIE_Msk;//RXNEIE
    //__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

    NVIC_SetVector(USART3_IRQn, (uint32_t)&serial_int);
    NVIC_EnableIRQ(USART3_IRQn);
    
    rasp.attach(&the_sonar, Serial::RxIrq);

    rasp.printf("Hello Ultrasound\n");

    tick_call.attach(&the_ticker,1);
    sonar.attach(&the_sonar,0.1);

    myled = 1; // LED is ON
    wait(1.0);

    myled = 0; // LED is OFF
    wait(1.0); // 1 sec

}



int main() 
{
    init();

    while(1) 
    {
        wait(1.0);
    }
}
