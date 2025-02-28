.section ".text.boot"

.globl _start
_start:
	mrs    x0, mpidr_el1   // get processor id from mpidr_el1 register
	and    x0, x0, #0xFF   // use and to check if id is zero
	cbz    x0, init
	b      loop

loop:
	wfe
	b      loop

init:
	# set top of stack
	ldr    x0, =_start
	mov    sp, x0
	# set bss
	adr    x0, bss_begin  // x0 store bss staring point
	adr    x1, bss_end
	sub    x1, x1, x0     // x1 store bss size
	cbz    x1, main       // if x1 == 0, then goto main
memzero:
	str    xzr, [x0], #8
	sub    x1, x1, #8
	cbnz   x1, memzero
main:
	bl     kernel_main
	b      loop