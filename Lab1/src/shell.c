#include "mini_uart.h"
#include "utils.h"
#include "mailbox.h"

void commands_list(void)
{
    mini_uart_send_string("help    : print this help menu\r\n");
    mini_uart_send_string("hello   : print Hello, World!\r\n");
    mini_uart_send_string("mailbox : print hardware's information\r\n");
}

void mailbox_status(void)
{
    mini_uart_send_string("Mailbox info:\r\n");
    mini_uart_send_string("Board revision:          ");
    get_board_revision();
    get_ARM_memory();
}
void execute_command(char *command)
{
    if (strcmp(command, "help") == 0)
    {
        commands_list();
    }
    else if (strcmp(command, "hello") == 0)
    {
        mini_uart_send_string("Hello, World!\r\n");
    }
    else if (strcmp(command, "mailbox") == 0)
    {
        mailbox_status();
    }
    else
    {
        mini_uart_send_string("Command not found, type 'help' for all commands.\r\n");
    }
}

void shell(void)
{
    while (1)
    {
        char command[100];
        mini_uart_send_string("# ");
        unsigned int idx = 0;
        // read the command line
        while (1)
        {
            char c = mini_uart_recv();
            mini_uart_send(c);
            if (c == '\r' || c == '\n')
            {
                command[idx] = '\0';
                break;
            }
            else
            {
                command[idx++] = c;
            }
        }
        // execute the command
        if (idx > 0)
        {
            execute_command(command);
        }
    }
}
