#include "peripherals/mailbox.h"
#include "mini_uart.h"
#include "utils.h"
volatile unsigned int __attribute__((aligned(16))) mailbox[8];

void mailbox_call(void)
{
    unsigned int combined_mailbox = (((unsigned int)((unsigned long)&mailbox) >> 4) << 4) | (0x8 & 0xF); // Combine the message address (upper 28 bits) with channel number(channel 8 => 0x8)
    while (*MAILBOX_STATUS & MAILBOX_FULL)                                                               // Check if Mailbox 0 status register’s full flag is set.
    {
        asm volatile("nop");
    }
    *MAILBOX_WRITE = combined_mailbox;
    while (1)
    {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) // Check if Mailbox 0 status register’s empty flag is set.
        {
            asm volatile("nop");
        }
        // read from Mailbox 0 Read/Write register
        if (combined_mailbox == *MAILBOX_READ && mailbox[1] == MAILBOX_RESPONSE) //  if the value is the same as combined_mailbox.(same address and channel)
        {
            break;
        }
    }
}
void get_board_revision(void)
{
    mailbox[0] = 7 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_BOARD_REVISION; // tag identifier
    mailbox[3] = 4;                  // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;
    mailbox_call();
    char output[12]; // String to store the result (10 characters + null terminator)
    hex_to_string(mailbox[5], output);
    mini_uart_send_string(output);
}
void get_ARM_memory(void)
{
    mailbox[0] = 8 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_ARM_MEMORY; // tag identifier
    mailbox[3] = 8;              // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = 0; // value buffer
    // tags end
    mailbox[7] = END_TAG;
    mailbox_call();
    char output[12]; // String to store the result (10 characters + null terminator)
    hex_to_string(mailbox[5], output);
    mini_uart_send_string("ARM memory base address: ");
    mini_uart_send_string(output);

    hex_to_string(mailbox[6], output);
    mini_uart_send_string("ARM memory size:         ");
    mini_uart_send_string(output);
}