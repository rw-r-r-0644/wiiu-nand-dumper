
.arm

.extern _app_terminate
.globl app_terminate

.section .text

app_terminate:
	@ Switch to a stack in SRAM
    ldr sp, =__sram_stack_addr
	
	@ Call the app_teminate function
    ldr r1, =_app_terminate
    blx r1
	