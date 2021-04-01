#pragma once
#include "common/types.h"

#define NAND_WRITE_ENABLED  0

/* page size in bytes */
#define PAGE_SIZE           0x800

/* nand pages count */
#define PAGE_COUNT          0x40000

/* block size in pages */
#define BLOCK_SIZE          0x40

/* nand block count */
#define BLOCK_COUNT         0x1000

/* nand spare size */
#define SPARE_SIZE          0x40

/* nand banks */
#define BANK_SLCCMPT        1
#define BANK_SLC            2

/* initialize nand */
void nand_initialize(u32 bank);

/* shutdown nand interface */
void nand_deinitialize(void);

/* read page and spare */
int nand_read_page(u32 pageno, void *data, void *spare);

#if NAND_WRITE_ENABLED
/* write page and spare */
int nand_write_page(u32 pageno, void *data, void *spare);

/* erase a block of pages */
int nand_erase_block(u32 blockno);
#endif

/* set enabled nand banks */
void nand_enable_banks(u32 bank);

/* nand irq handler */
void nand_irq(void);
