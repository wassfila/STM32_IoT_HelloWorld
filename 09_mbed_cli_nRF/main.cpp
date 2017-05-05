#include "mbed.h"
#include "nRF24L01P.h"

Serial pc(PB_10, PB_11, 115200);

nRF24L01P my_nrf24l01p(PB_15, PB_14, PB_13, PB_12, PA_8, PA_9);    // mosi, miso, sck, csn, ce, irq

DigitalOut myled1(LED1);
//DigitalOut myled2(LED2);

void nrf_print_info(nRF24L01P &nrf)
{
    // Display the (default) setup of the nRF24L01+ chip
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",    nrf.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",    nrf.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n",   nrf.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", nrf.getTxAddress() );
    pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", nrf.getRxAddress() );
    
}

int main() 
{
    pc.printf( "Hello world from the nRF Sample\r\n");
    wait(1.0); // 1 sec

// The nRF24L01+ supports transfers from 1 to 32 bytes, but Sparkfun's
//  "Nordic Serial Interface Board" (http://www.sparkfun.com/products/9019)
//  only handles 4 byte transfers in the ATMega code.
#define TRANSFER_SIZE   32

    char txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
    int txDataCnt = 0;
    int rxDataCnt = 0;

    pc.printf("powerUp()\r\n");
    my_nrf24l01p.powerUp();

    nrf_print_info(my_nrf24l01p);

    pc.printf("setAirDataRate()\r\n");
    my_nrf24l01p.setAirDataRate(NRF24L01P_DATARATE_2_MBPS);
    pc.printf("setTxAddress()\r\n");
    my_nrf24l01p.setTxAddress(DEFAULT_NRF24L01P_ADDRESS,5);
    pc.printf("setCrcWidth()\r\n");
    my_nrf24l01p.setCrcWidth(NRF24L01P_CRC_NONE);

    nrf_print_info(my_nrf24l01p);

    pc.printf( "Type keys to test transfers:\r\n  (transfers are grouped into %d characters)\r\n", TRANSFER_SIZE );

    my_nrf24l01p.setTransferSize( TRANSFER_SIZE );

    my_nrf24l01p.setReceiveMode();
    my_nrf24l01p.enable();

    while (1) {

        // If we've received anything over the host serial link...
        if ( pc.readable() ) {

            // ...add it to the transmit buffer
            txData[txDataCnt++] = pc.getc();

            // If the transmit buffer is full
            if ( txDataCnt >= sizeof( txData ) ) {

                // Send the transmitbuffer via the nRF24L01+
                my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );

                txDataCnt = 0;
            }

            // Toggle LED1 (to help debug Host -> nRF24L01+ communication)
            myled1 = !myled1;
        }

        // If we've received anything in the nRF24L01+...
        if ( my_nrf24l01p.readable() ) {

            // ...read the data into the receive buffer
            rxDataCnt = my_nrf24l01p.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );

            // Display the receive buffer contents via the host serial link
            pc.printf("Rx : 0x");
            for ( int i = 0; rxDataCnt > 0; rxDataCnt--, i++ ) {

                //pc.putc( rxData[i] );
                pc.printf(" %x",rxData[i]);
            }
            pc.printf("\r\n");

        }
    }
}
