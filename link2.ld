OUTPUT_FORMAT(binary)
ENTRY(start)
SECTIONS
{
  . = 0x7000;
  .text : {
    *(.text) *(.text.*)
	}
    .data : {
		*(.rodata) *(.rodata.*)
		*(.data) *(.data.*)
		*(.gcc_except_table)
	}
    .bss (NOLOAD) : {
                *(.bss) *(.bss.*)
                *(COMMON)
	}
    /DISCARD/ : { *(.*) }
}