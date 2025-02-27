#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"
#include "utils.h"

void mini_uart_init(void){
    unsigned int selector;

    // setting GPFSEL1 register
    selector = *GPFSEL1;
    selector &= ~(7 << 12);                   // set gpio14 to 000
	selector |= 2 << 12;                      // set alt5 (010) for gpio14
	selector &= ~(7 << 15);                   // set gpio14 to 000
	selector |= 2 << 15;                      // set alt5 (010) for gpio15
    *GPFSEL1 = selector;                      // write back to GPFSEL1 register
    // GPIO Pull-up/down
    *GPPUD = 0;
    delay(150u);
    *GPPUDCLK0 = (1 << 14) | (1 << 15);       // clock the control signal for gpio14 and gpio15
    delay(150u);
    *GPPUD = 0;                               // remove the control signal
    *GPPUDCLK0 = 0;                           // remove the clock
    // initialize mini-uart
    *AUX_ENABLES = 1;                         //Enable mini uart (this also enables access to its registers)
    *AUX_MU_CNTL_REG = 0;                     //Disable auto flow control and disable receiver and transmitter (for now)
	*AUX_MU_IER_REG = 0;                      // Disable receive and transmit interrupts
    *AUX_MU_LCR_REG = 3;                      // Enable 8 bit mode
    *AUX_MU_MCR_REG = 0;                      // Set RTS line to be always high
    *AUX_MU_BAUD_REG = 270;                   // Set baud rate to 115200
    *AUX_MU_IIR_REG = 6;                      // disable interrupts
    *AUX_MU_CNTL_REG = 3;                     // Finally, enable transmitter and receiver
}

void mini_uart_send ( char c )
{
    while(1) {
        // if xxxx xxxx xx1x xxxx => Write
        if((*(AUX_MU_LSR_REG)&0x20)) 
            break;
    }
    *(AUX_MU_IO_REG) = c;
}

char mini_uart_recv ( void )
{
    // if xxxx xxxx xxxx xxx1 => Read
    while(1) {
        if((*(AUX_MU_LSR_REG)&0x01)) 
            break;
    }
    // maybe try add \r\n? 
    return(*(AUX_MU_IO_REG)&0xFF);
}

void mini_uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		mini_uart_send((char)str[i]);
	}
}