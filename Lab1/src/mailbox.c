#include "peripherals/mailbox.h"
#include "mini_uart.h"
#include "utils.h"

void mailbox_call(unsigned int *mailbox)
{
    // Combine the message address (upper 28 bits) with channel number(channel 8 => 0x8)
    unsigned int combined_mailbox = (((unsigned int)((unsigned long)&mailbox) & ~0xF) | (0x8 & 0xF));
    // Check if Mailbox 0 status register’s full flag is set.
    while (*MAILBOX_STATUS & MAILBOX_FULL)
    {
        asm volatile("nop");
    }
    // write to Mailbox 1 Read/Write register
    *MAILBOX_WRITE = combined_mailbox;
    // Check if Mailbox 0 status register’s empty flag is set.
    while (1)
    {
        // Check if Mailbox 0 status register’s empty flag is set.
        while (*MAILBOX_STATUS & MAILBOX_EMPTY)
        {
            asm volatile("nop");
        }
        //  if the value is the same as combined_mailbox.(same address and channel)
        if (combined_mailbox == *MAILBOX_READ && MAILBOX_READ[1] == REQUEST_SUCCEED)
        {
            *mailbox = *MAILBOX_READ;
        }
        // read from Mailbox 0 Read/Write register
    }
}
void get_board_revision(void)
{
    unsigned int mailbox[7];
    mailbox[0] = 7 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_BOARD_REVISION; // tag identifier
    mailbox[3] = 4;                  // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;

    char output[12]; // String to store the result (10 characters + null terminator)
    hex_to_string(mailbox[5], output);
    mini_uart_send_string(output); // message passing procedure call, you should implement it following the 6 steps provided above.
}