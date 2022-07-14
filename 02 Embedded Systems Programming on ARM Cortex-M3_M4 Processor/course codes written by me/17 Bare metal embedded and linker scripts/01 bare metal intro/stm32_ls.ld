ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx):ORIGIN =0x08000000,LENGTH =1024K
	SRAM(rwx):ORIGIN =0X20000000,LENGTH =128K
}

SECTIONS
{
	.text :
	{
		*(.isr_vector)
		*(.text)
		*(.text.*)
		*(.init)
		*(.fini)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
		end_of_text = .; 
	}> FLASH AT> FLASH
	
	_la_data = LOADADDR(.data);
	.data :
	{
	_start_of_data = .;
		*(.data)
		*(.data.*)
	. = ALIGN(4);
	_end_of_data = .;
	}> SRAM AT>FLASH
	
	.bss :
	{
	_start_of_bss = .;
	__bss_start__ = _start_of_bss;
		*(.bss)
		*(.bss.*)
		*(COMMON)
	. = ALIGN(4);
	_end_of_bss = .;
	__bss_end__ = _end_of_bss;
	. = ALIGN(4);
	end = .;
	__end__ = .;
	}> SRAM
}