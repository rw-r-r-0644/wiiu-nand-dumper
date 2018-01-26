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
#include "system/irq.h"
#include "system/smc.h"
#include "storage/isfs.h"
#include "common/utils.h"
#include "storage/sd/sdcard.h"
#include "storage/sd/fatfs/elm.h"

#define PPCMSG (u32)0x0d800000
#define PPCCTRL (u32)0x0d800004
#define ARMCTRL (u32)0x0d80000c
#define ARMCTRL_X1 0x4
#define ARMCTRL_Y1 0x1

u32 SRAM_DATA ppc_entry = 0;

void app_run() {
	smc_wait_events(SMC_POWER_BUTTON);

	int res = ppc_load_file("sdmc:/ppc-kernel.elf", &ppc_entry);
	if (res < 0) {
		printf("[FATL] Loading PowerPC kernel failed! (%d)\n", res);
		panic(0);
	}
	printf("[ OK ] Loaded PowerPC kernel (%d). Entry is %08lX.\n", res, ppc_entry);

	ELM_Unmount();
	sdcard_exit();
	irq_disable(IRQ_SD0);
	printf("[ OK ] Unmounted SD\n");
	
	udelay(1000000);
}

#define _READ32(addr, data) __asm__ volatile ("ldr\t%0, [%1]" : "=l" (data) : "l" (addr))
#define _WRITE32(addr, data) __asm__ volatile ("str\t%0, [%1]" : : "l" (data), "l" (addr))
#define _PPC_JUMP(entry) _WRITE32(0x14000000, entry)

// Calling functions outside of SRAM in this function will result in a crash
void SRAM_TEXT __attribute__((__noreturn__)) _app_terminate() {
	// Start the kernel on the PPC
	_PPC_JUMP(ppc_entry);

	while (1) {
		u32 ctrl, cmd;
		
		// Read ARMCTRL -> ctrl
		_READ32(ARMCTRL, ctrl);
		
		if (ctrl & ARMCTRL_X1) {
			// Read PPCMSG -> cmd
			_READ32(PPCMSG, cmd);
			
			// 0xCAFE0001: Poweroff command
			if (cmd == 0xCAFE0001) {
				// (smc_shutdown is in SRAM)
				smc_shutdown(false);
			}
			
			// 0xCAFE0002: Reboot command
			if (cmd == 0xCAFE0002) {
				// (smc_shutdown is in SRAM)
				smc_shutdown(true);
			}
			
			_WRITE32(ARMCTRL, ctrl);
		}
	}
}