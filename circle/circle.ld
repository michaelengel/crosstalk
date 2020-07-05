/*
 * circle.ld
 */

ENTRY(_start)

SECTIONS
{
	.init : {
		*(.init)
	}

	.text : {
		*(.text*)

		_etext = .;
	}

	.rodata : {
		*(.rodata*)
	}

	.init_array : {
		__init_start = .;

		*(.init_array*)

		__init_end = .;
	}

	.ARM.exidx : {
		__exidx_start = .;

		*(.ARM.exidx*)

		__exidx_end = .;
	}

	.eh_frame : {
		*(.eh_frame*)
	}

	.data : {
		*(.data*)
	}

	.bss : {
	        __bss_start__ = ABSOLUTE(.);
		__bss_start = .;

		*(.bss*)
		*(COMMON)

		_end = .;
        	__bss_end__ = ABSOLUTE(.);
	}

    . = ALIGN(8);
    .stack : {
        __stack_start__ = ABSOLUTE(.);

        . += 262144;
        __c_stack_top__ = ABSOLUTE(.);

        __stack_end__ = ABSOLUTE(.);
    }

    .heap : {
        __heap_start = ABSOLUTE(.);
        __heap_end = 0x1C000000;
    }

}
