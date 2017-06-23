/*
 *	It's a Project! linux-loader
 *
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
#include "system/smc.h"
#include "video/gfx.h"
#include "system/ppc_elf.h"
#include "storage/isfs.h"

void app_run() {
	smc_wait_events(SMC_POWER_BUTTON);

	u32 ppc_entry = 0;
	int res = ppc_load_file("sdmc:/ppc-kernel.elf", &ppc_entry);

	printf("[ OK ] Loaded PowerPC kernel (%d). Entry is %08lX.\n", res, ppc_entry);

	/*printf("Attempting to open ppc-kernel.elf...");
	FILE* fd = fopen("sdmc:/ppc-kernel.elf", "rb");
	printf("fd: %d\n", (int)fd);*/

	//isfs_test();

	//fclose(fd);

	printf("--------------------------\n");
	printf("         Finished         \n");
	printf("--------------------------\n");
	smc_wait_events(SMC_POWER_BUTTON);
}
