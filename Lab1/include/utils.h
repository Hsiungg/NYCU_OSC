void delay(unsigned int cycles){
    while(cycles--) { asm volatile("nop"); }
}