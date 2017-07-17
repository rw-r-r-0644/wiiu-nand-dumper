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
#include "system/ppc.h"
#include "system/memory.h"
#include "storage/isfs.h"
#include "common/utils.h"

#define PPCMSG (u32)0x0d800000
#define ARMCTRL (u32)0x0d80000c
#define ARMCTRL_X1 0x4

void app_run() {
	smc_wait_events(SMC_POWER_BUTTON);

	u32 ppc_entry = 0;
	int res = ppc_load_file("sdmc:/ppc-kernel.elf", &ppc_entry);
	if (res < 0) {
		printf("[FATL] Loading PowerPC kernel failed! (%d)\n", res);
		panic(0);
	}
	printf("[ OK ] Loaded PowerPC kernel (%d). Entry is %08lX.\n", res, ppc_entry);

	printf("--------------------------\n");
	printf("   Spinning up PowerPC!   \n");
	printf("--------------------------\n");

	udelay(1000000);

	ppc_jump(ppc_entry);

	printf("[ OK ] Span up PowerPC.\n");

	int x = 0, y = 0;
	for (;;) {
		if (smc_get_events() & SMC_EJECT_BUTTON) break;
		dc_invalidaterange((void*)PPCMSG, 0x100);
		u32 ctrl = read32(ARMCTRL);
		if (!(ctrl & ARMCTRL_X1)) {
			udelay(1);
			continue;
		}

		char c = (char)(read32(PPCMSG) & 0xFF);
		if (!c) {
			write32(ARMCTRL, ctrl);
			continue;
		}
		if (c == '\n') {
			x = 0;
			y += 8;
			if (y > 700) {
				y = 0;
			}
			write32(ARMCTRL, ctrl);
			continue;
		}
		if (c < 32 || c >= 128) {
			write32(ARMCTRL, ctrl);
			continue;
		}
		//printf("got char: %X\n", c);
		gfx_draw_char(GFX_TV, c, x, y, GREEN);
		x += 8;
		if (x > 1250) {
			x = 0;
			y += 8;
			if (y > 700) {
				y = 0;
			}
		}
		write32(ARMCTRL, ctrl);
	}

	printf("done\n");

	smc_wait_events(SMC_POWER_BUTTON);
	gfx_clear(GFX_ALL, BLACK);
}
