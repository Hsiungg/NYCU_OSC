#include "mini_uart.h"
#include "shell.h"
void kernel_main(void)
{
    mini_uart_init();
    mini_uart_send_string("Welcome to OSC Lab1!\r\n");
    shell();
}