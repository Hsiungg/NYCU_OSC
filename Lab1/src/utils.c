void delay(unsigned int cycles)
{
    while (cycles--)
    {
        asm volatile("nop");
    }
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    // return how many characters are different between s1 and s2
    return *s1 - *s2;
}

void hex_to_string(unsigned int hex_value, char *output)
{
    // Characters for hexadecimal digits
    char hex_chars[] = "0123456789ABCDEF";
    // Start the string with "0x"
    output[0] = '0';
    output[1] = 'x';

    // Convert each of the 8 hex digits
    for (int i = 7; i >= 0; i--)
    {
        int nibble = (hex_value >> (i * 4)) & 0xF; // Get each 4 bits (nibble)
        output[2 + (7 - i)] = hex_chars[nibble];   // Map to hex char
    }

    output[10] = '\r';
    output[11] = '\n';
}