#include "mini_uart.h"
//#include "shell.h"
void kernel_main(void){
    mini_uart_init();
    mini_uart_send_string("Hello, world!\r\n");
    while (1) {
		mini_uart_send(mini_uart_recv());
	}
    //shell_init();
}