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


void progress(int percent) {
	static char progressbar[100];
	if (percent > 100) percent = 100;
	memset(progressbar, '.', 100);
	memset(progressbar, '#', percent);
	printf("%03d%% [%s]", percent, progressbar);
}

void dump_otp() {
	FILE* f_otp = fopen("sdmc:/otp.bin", "wb+");
	if(!f_otp)
		return;
	printf("Dumping OTP ...\n");
	fwrite(&otp, 1, sizeof(otp_t), f_otp);
	fclose(f_otp);
	progress(100);
	printf("\n\n");
}

void dump_seeprom()
{
	FILE* f_eep = fopen("sdmc:/seeprom.bin", "wb+");
	if(!f_eep)
		return;
	printf("Dumping SEEPROM ...\n");
	fwrite(&seeprom, 1, sizeof(seeprom_t), f_eep);
	fclose(f_eep);
	progress(100);
	printf("\n\n");
}

#define NAND_PAGES_PER_ITERATION (0x20)	
void dump_nand(FILE *f, u32 bank)
{
	static u8 page_buf[PAGE_SIZE] ALIGNED(64);
	static u8 ecc_buf[ECC_BUFFER_ALLOC] ALIGNED(128);
	
	static u8 file_buf[NAND_PAGES_PER_ITERATION][PAGE_SIZE + PAGE_SPARE_SIZE];
	
	u32 last_percent = (u32)-1;

	nand_initialize(bank);

	for(u32 i = 0; i < (NAND_MAX_PAGE / NAND_PAGES_PER_ITERATION); i++)
	{
		u32 page_base = i * NAND_PAGES_PER_ITERATION;

		u32 percent = (page_base * 100) / NAND_MAX_PAGE;
		if(percent != last_percent) {
			progress(percent);
			last_percent = percent;
		}
		
		for(u32 page = 0; page < NAND_PAGES_PER_ITERATION; page++)
		{
			nand_read_page(page_base + page, page_buf, ecc_buf);
			nand_wait();
			nand_correct(page_base + page, page_buf, ecc_buf);
			
			memcpy(file_buf[page], page_buf, PAGE_SIZE);
			memcpy(file_buf[page] + PAGE_SIZE, ecc_buf, PAGE_SPARE_SIZE);
		}
		
		if(fwrite(file_buf, 1, sizeof(file_buf), f) != sizeof(file_buf))
			return;
	}
	progress(100);
	printf("\n\n");
	
	return 0;
}

void dump_slc()
{
	FILE* f_slc = fopen("sdmc:/slc.bin", "wb+");
	if(!f_slc)
		return;
	printf("Dumping SLC ...\n");
	dump_nand(f_slc, NAND_BANK_SLC);
	fclose(f_slc);
}

void dump_slccmpt()
{
	FILE* f_slccmpt = fopen("sdmc:/slccmpt.bin", "wb+");
	if(!f_slccmpt)
		return;
	printf("Dumping SLCCMPT ...\n");
	dump_nand(f_slccmpt, NAND_BANK_SLCCMPT);
	fclose(f_slccmpt);
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
