/*
 *	wiiu-nand-dumper
 * 
 *	Copyright (C) 2020          rw-r-r-0644 <rwrr0644@gmail.com>
 *	Copyright (C) 2017          Ash Logan <quarktheawesome@gmail.com>
 *
 *	Based on code from the following contributors:
 *
 *	Copyright (C) 2016          SALT
 *	Copyright (C) 2016          Daz Jones <daz@dazzozo.com>
 *
 *	Copyright (C) 2008, 2009    Haxx Enterprises <bushing@gmail.com>
 *	Copyright (C) 2008, 2009    Sven Peter <svenpeter@gmail.com>
 *	Copyright (C) 2008, 2009    Hector Martin "marcan" <marcan@marcansoft.com>
 *	Copyright (C) 2009          Andre Heider "dhewg" <dhewg@wiibrew.org>
 *	Copyright (C) 2009          John Kelley <wiidev@kelley.ca>
 *
 *	(see https://github.com/Dazzozo/minute)
 *
 *	This code is licensed to you under the terms of the GNU GPL, version 2;
 *	see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "video/gfx.h"
#include "system/smc.h"
#include "system/memory.h"
#include "system/irq.h"
#include "system/smc.h"
#include "system/latte.h"
#include "storage/nand.h"
#include "storage/crypto.h"
#include "common/utils.h"
#include "storage/sd/sdcard.h"
#include "storage/sd/fatfs/elm.h"
#include "application.h"


void progress(u32 current, u32 max) {
	static char progressbar[64];
	static u32 last = 0;
	if (current > max) {
		current = max;
	}
	u32 percent = (current * 100) / max;
	if (percent == last) {
		return;
	}
	last = percent;
	memset(progressbar, '.', sizeof(progressbar));
	memset(progressbar, '#', (current * sizeof(progressbar)) / max);
	printf("%03d%% [%s]", percent, progressbar);
}

void dump_otp() {
	FILE* f_otp = fopen("sdmc:/otp.bin", "wb");
	if(!f_otp)
		return;
	printf("Dumping OTP ...\n");
	progress(0, 100);
	fwrite(&otp, 1, sizeof(otp_t), f_otp);
	fclose(f_otp);
	progress(100, 100);
	printf("\n\n");
}

void dump_seeprom()
{
	FILE* f_eep = fopen("sdmc:/seeprom.bin", "wb");
	if(!f_eep)
		return;
	printf("Dumping SEEPROM ...\n");
	progress(0, 100);
	fwrite(&seeprom, 1, sizeof(seeprom_t), f_eep);
	fclose(f_eep);
	progress(100, 100);
	printf("\n\n");
}

#define NAND_PAGES_PER_ITERATION (0x20)	
void dump_nand(FIL *f, u32 bank)
{
	static u8 file_buf[NAND_PAGES_PER_ITERATION][PAGE_SIZE + SPARE_SIZE] ALIGNED(128);

	FRESULT fres = 0; UINT btx = 0;

	nand_initialize(bank);

	for(u32 i = 0; i < (PAGE_COUNT / NAND_PAGES_PER_ITERATION); i++)
	{
		u32 page_base = i * NAND_PAGES_PER_ITERATION;

		progress(page_base, PAGE_COUNT);
		
		for(u32 page = 0; page < NAND_PAGES_PER_ITERATION; page++)
			nand_read_page(page_base + page, file_buf[page], file_buf[page] + PAGE_SIZE);
		
		fres = f_write(f, file_buf, sizeof(file_buf), &btx);
		if(fres != FR_OK || btx != sizeof(file_buf))
		{
			f_close(f);
			return;
		}
	}
	progress(100, 100);
	printf("\n\n");
}

void dump_slc()
{
	FIL f_slc = {0};
	if(f_open(&f_slc, "slc.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
		return;
	printf("Dumping SLC ...\n");
	dump_nand(&f_slc, BANK_SLC);
	f_close(&f_slc);
}

void dump_slccmpt()
{
	FIL f_slccmpt = {0};
	if(f_open(&f_slccmpt, "slccmpt.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
		return;
	printf("Dumping SLCCMPT ...\n");
	dump_nand(&f_slccmpt, BANK_SLCCMPT);
	f_close(&f_slccmpt);
}

void app_run()
{
	gfx_clear(GFX_ALL, BLACK);
	
	printf("WIIU-NAND-DUMPER\n\n");

	dump_otp();
	dump_seeprom();
	dump_slc();
	dump_slccmpt();

	printf("Done! Press POWER to poweroff...\n");
	smc_wait_events(SMC_POWER_BUTTON);
}
