/*
 *	It's a Project! linux-loader
 *
 *	Copyright (C) 2017 - Ash Logan <quarktheawesome@gmail.com>
 *
 *	This code is licensed to you under the terms of the GNU GPL, version 2;
 *	see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include "ppc-asm.h"

/*	Relative to the ELF base address, thus _start will be at 0x1000.
 *	Sure, I *could* use the ELF entrypoint, but this is easier.
 */
.org 0

/*	Test function; just writes 0xBABE.
 *	Not much to say here.
 */
.globl _start
start:
	li r3, 0
	li r4, 0xBABE@l
	stw r4, 0(r3)
	b .
