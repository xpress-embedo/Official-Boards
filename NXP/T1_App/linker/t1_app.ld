

GROUP (
  "libcr_nohost_nf.a"
  "libcr_c.a"
  "libcr_eabihelpers.a"
  "libgcc.a"
)

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x40000   /* 256K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x1fffc000, LENGTH = 0xc000     /* 48K bytes (alias RAM) */  
}

/* Define a symbol for the top of each memory region */
__base_PROGRAM_FLASH = 0x0  ;           /* PROGRAM_FLASH */  
__base_Flash = 0x0 ;                    /* Flash */  
__top_PROGRAM_FLASH = 0x0 + 0x40000 ;   /* 256K bytes */  
__top_Flash = 0x0 + 0x40000 ;           /* 256K bytes */  
__base_SRAM = 0x1fffc000  ;             /* SRAM */  
__base_RAM = 0x1fffc000 ;               /* RAM */  
__top_SRAM = 0x1fffc000 + 0xc000 ;      /* 48K bytes */  
__top_RAM = 0x1fffc000 + 0xc000 ;       /* 48K bytes */  


ENTRY(ResetISR)

SECTIONS
{
  .text :
  {
    *(.isr_vector)
    *(.text)
    *(.rodata)
  } > PROGRAM_FLASH
  
  .data :
  {
    *(.data)
  } > SRAM AT> PROGRAM_FLASH
  
  .bss :
  {
    *(.bss)
  } > SRAM
}