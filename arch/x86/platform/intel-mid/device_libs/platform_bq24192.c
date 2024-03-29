/*
 * platform_bq24192.c: bq24192 platform data initilization file
 *
 * (C) Copyright 2008 Intel Corporation
 * Author:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/power/bq24192_charger.h>
#include <linux/lnw_gpio.h>
#include <linux/power_supply.h>
#include <asm/intel-mid.h>
#include <asm/intel_mid_remoteproc.h>
#include "platform_bq24192.h"

static struct power_supply_throttle bq24192_throttle_states[] = {
	{
		/* USER_SET_CHRG_DISABLE */
		.throttle_action = PSY_THROTTLE_DISABLE_CHARGER,
	},
	{
		/* USER_SET_CHRG_LMT1 */
		.throttle_action = PSY_THROTTLE_CC_LIMIT,
		.throttle_val = BQ24192_CHRG_CUR_LOW

	},
	{
		/* USER_SET_CHRG_LMT2 */
		.throttle_action = PSY_THROTTLE_CC_LIMIT,
		.throttle_val = BQ24192_CHRG_CUR_MEDIUM

	},
	{
		/* USER_SET_CHRG_LMT3 */
		.throttle_action = PSY_THROTTLE_CC_LIMIT,
		.throttle_val = BQ24192_CHRG_CUR_HIGH
	},
	{
		/* USER_SET_CHRG_NOLMT */
		.throttle_action = PSY_THROTTLE_CC_LIMIT,
		.throttle_val = BQ24192_CHRG_CUR_NOLIMIT
	},
};

static bool msic_battery_check(void)
{
	if (get_oem0_table() == NULL) {
		pr_info("Invalid battery detected\n");
		return false;
	} else {
		pr_info("Valid battery detected\n");
		return true;
	}
}

void *bq24192_platform_data(void *info)
{
	static struct bq24192_platform_data platform_data;

	if (msic_battery_check())
		platform_data.sfi_tabl_present = true;
	else
		platform_data.sfi_tabl_present = false;

	platform_data.throttle_states = bq24192_throttle_states;
	platform_data.num_throttle_states = ARRAY_SIZE(bq24192_throttle_states);

	/* Define the temperature ranges */
	platform_data.temp_mon_ranges = 4;
	platform_data.temp_mon_range[0].temp_up_lim = 60;
	platform_data.temp_mon_range[0].full_chrg_vol = 4100;
	platform_data.temp_mon_range[0].full_chrg_cur = 1500;
	platform_data.temp_mon_range[0].maint_chrg_vol_ll = 4000;
	platform_data.temp_mon_range[0].maint_chrg_vol_ul = 4050;
	platform_data.temp_mon_range[0].maint_chrg_cur = 950;

	platform_data.temp_mon_range[1].temp_up_lim = 45;
	platform_data.temp_mon_range[1].full_chrg_vol = 4200;
	platform_data.temp_mon_range[1].full_chrg_cur = 1500;
	platform_data.temp_mon_range[1].maint_chrg_vol_ll = 4126;
	platform_data.temp_mon_range[1].maint_chrg_vol_ul = 4200;
	platform_data.temp_mon_range[1].maint_chrg_cur = 950;

	platform_data.temp_mon_range[2].temp_up_lim = 10;
	platform_data.temp_mon_range[2].full_chrg_vol = 4200;
	platform_data.temp_mon_range[2].full_chrg_cur = 1500;
	platform_data.temp_mon_range[2].maint_chrg_vol_ll = 4126;
	platform_data.temp_mon_range[2].maint_chrg_vol_ul = 4200;
	platform_data.temp_mon_range[2].maint_chrg_cur = 950;

	platform_data.temp_mon_range[3].temp_up_lim = 0;
	platform_data.temp_mon_range[3].full_chrg_vol = 3950;
	platform_data.temp_mon_range[3].full_chrg_cur = 950;
	platform_data.temp_mon_range[3].maint_chrg_vol_ll = 3900;
	platform_data.temp_mon_range[3].maint_chrg_vol_ul = 3950;
	platform_data.temp_mon_range[3].maint_chrg_cur = 950;

	platform_data.temp_low_lim = -10;
	platform_data.slave_mode = 0;

	register_rpmsg_service("rpmsg_bq24192", RPROC_SCU,
				RP_BQ24192);
	/* WA for pmic rpmsg service registration
	   for power source detection driver */
	register_rpmsg_service("rpmsg_pmic_charger", RPROC_SCU,
				RP_PMIC_CHARGER);
	return &platform_data;
}
