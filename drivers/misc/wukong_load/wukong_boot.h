/******************************************************************************
*(C) Copyright 2011 Marvell International Ltd.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  Filename: wukong_boot.h
 *
 *  Description: export wukong boot externel structure defenition and API.
 *
 *  History:
 *   Nov, 11 2011 -  Li Wang(wangli@marvell.com) Creation of file
 *
 *  Notes:
 *
 ******************************************************************************/
#ifndef __WUKONG_BOOT_H_
#define __WUKONG_BOOT_H_

#include "wukong_load.h"

enum modemState
{
	HOLD = 0,
	RELEASE,
	BOOTUP,
	UNKNOWN_STATE
};


int init_modem_boot(struct modem_device * modem);
void deinit_modem_boot(struct modem_device * modem);

enum modemState get_modem_state(void);
char* get_modem_state_name(void);

void hold_modem(int internal);
void release_modem(int internal);

void uart_dev_open(void);
void uart_dev_close(void);

#endif
