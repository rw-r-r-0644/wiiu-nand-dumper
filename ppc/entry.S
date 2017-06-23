/*
 *	It's a Project! linux-loader
 *
 *	Copyright (C) 2017 - Ash Logan <quarktheawesome@gmail.com>
 *
 *	This code is licensed to you under the terms of the GNU GPL, version 2;
 *	see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include "ppc-asm.h"

/*	Test function; just writes 0xBABE.
 *	Not much to say here.
 */
.globl _start
_start:
	li r3, 0
	li r4, 0xBABE@l
	stw r4, 0(r3)
	b .
