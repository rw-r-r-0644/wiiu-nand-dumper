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

#include <stdlib.h>
#include "video/gfx.h"
#include "system/ppc.h"
#include "system/exception.h"
#include "system/memory.h"
#include "system/irq.h"
#include "storage/sd/sdcard.h"
#include "storage/sd/fatfs/elm.h"
#include "storage/isfs.h"
#include "storage/crypto.h"
#include "application.h"
#include "system/smc.h"
#include "system/latte.h"
#include "common/utils.h"

void NORETURN _main(void* base) {
	exception_initialize();
	mem_initialize();
	irq_initialize();
	srand(read32(LT_TIMER));
	crypto_initialize();
	sdcard_init();
	int res = ELM_Mount();
	if (res) {
		printf("SD Card mount error: %d\n", res);
		panic(0);
	}

	app_run();

	ELM_Unmount();
	sdcard_exit();
	irq_disable(IRQ_SD0);
	isfs_fini();
	irq_shutdown();
	mem_shutdown();
	smc_shutdown(false);
}
