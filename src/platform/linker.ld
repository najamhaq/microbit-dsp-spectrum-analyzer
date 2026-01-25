ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
  } > FLASH

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
      _sdata = .;         /* start of .data in RAM */
      *(.data*)
      _edata = .;         /* end of .data in RAM */
    } > RAM

    /* Start address of init values for .data in FLASH */
    _sidata = LOADADDR(.data);

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM
}
