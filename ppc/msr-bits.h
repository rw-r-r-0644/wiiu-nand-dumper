/*
 *	It's a Project! linux-loader
 *
 *	Copyright (C) 2017 - Ash Logan <quarktheawesome@gmail.com>
 *
 *	This code is licensed to you under the terms of the GNU GPL, version 2;
 *	see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

/*	PowerPC MSR bits.
 *	Taken from the 750CL User's Manual (page 163); though any
 *	good architecture spec should have these
 */

#define MSR_POW	(1 << 13)
#define MSR_ILE	(1 << 15)
#define MSR_EE	(1 << 16)
#define MSR_PR	(1 << 17)
#define MSR_FP	(1 << 18)
#define MSR_ME	(1 << 19)
#define MSR_FE0	(1 << 20)
#define MSR_SE	(1 << 21)
#define MSR_BE	(1 << 22)
#define MSR_FE1	(1 << 23)
#define MSR_IP	(1 << 25)
#define MSR_IR	(1 << 26)
#define MSR_DR	(1 << 27)
#define MSR_PM	(1 << 29) //750CL only
#define MSR_RI	(1 << 30)
#define MSR_LE	(1 << 31)
