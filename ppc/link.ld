ENTRY(_start);

SECTIONS {
	. = 0x1000;
	.text : {
		*(.text*);
	}
	.rodata : {
		*(.rodata*);
	}
	.data : {
		*(.data*);

		__sdata_start = .;
		*(.sdata*);
		__sdata_end = .;

		__sdata2_start = .;
		*(.sdata2*);
		__sdata2_end = .;
	}
	.bss : {
		__bss_start = .;
		*(.bss*);
		*(.sbss*);
		*(COMMON);
		__bss_end = .;
	}

	/DISCARD/ : {
		*(*);
	}
}
