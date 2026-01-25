.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler
.global Timer_Handler
.extern main
.extern SysTick_Handler

/* 128 KB RAM @ 0x20000000 -> top = 0x20020000 */
.equ _estack, 0x20020000

.section .isr_vector, "a", %progbits
g_pfnVectors:
  .word _estack
  .word Reset_Handler + 1    /* <-- IMPORTANT: Thumb bit set */
  .word Default_Handler + 1
  .word Default_Handler + 1
  .word Default_Handler + 1
  .word Default_Handler + 1
  .word Default_Handler + 1
  .word 0
  .word 0
  .word 0
  .word 0
  .word vPortSVCHandler + 1
  .word DebugMon_Handler + 1
  .word 0
  .word xPortPendSVHandler + 1
  .word xPortSysTickHandler + 1

  /* IRQs */
  .rept 48
    .word Default_Handler + 1
  .endr

.section .text.Reset_Handler, "ax", %progbits
.thumb_func
Reset_Handler:
    /* Copy .data from flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
1:
    cmp r0, r1
    bcc 2f
    b 3f
2:
    ldr r3, [r2], #4
    str r3, [r0], #4
    b 1b

3:
    /* Zero .bss */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
4:
    cmp r0, r1
    bcc 5f
    b 6f
5:
    str r2, [r0], #4
    /* remove: adds r0, r0, #4 */
    b 4b

6:
    /* Optionally set VTOR (highly recommended) */
    ldr r0, =0xE000ED08      /* SCB->VTOR */
    ldr r1, =g_pfnVectors
    str r1, [r0]

    /* Call main */
    bl main

7:
    b 7b

.section .text.Default_Handler, "ax", %progbits
.thumb_func
Default_Handler:
  b .
