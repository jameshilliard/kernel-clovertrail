/*
 * platform_ctp_audio.c: CLVS audio platform data initilization file
 *
 * (C) Copyright 2008-2013 Intel Corporation
 * Author: KP Jeeja<jeeja.kp@intel.com>
 * Author: Dharageswari.R<dharageswari.r@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/scatterlist.h>
#include <linux/init.h>
#include <linux/sfi.h>
#include <linux/platform_device.h>
#include <linux/jack.h>
#include <asm/intel-mid.h>
#include <asm/intel_mid_remoteproc.h>
#include <asm/platform_sst_audio.h>
#include <asm/platform_ctp_audio.h>
#include "platform_msic.h"

static struct ctp_audio_platform_data ctp_audio_pdata = {
	.spid = &spid,
};

#ifdef CONFIG_JACK_MON
static struct jack_platform_data jack_data = {
	.usb_online             = 0,
	.charger_online         = -1,
	.hdmi_online            = -1,
	.earjack_online         = 0,
	.earkey_online          = 0,
	.ums_online             = -1,
	.cdrom_online           = -1,
	.jig_online             = -1,
};
#endif

void *ctp_audio_platform_data(void *info)
{
	struct platform_device *pdev;
	int ret;
	struct sfi_device_table_entry *pentry = info;

	ctp_audio_pdata.codec_gpio_hsdet = get_gpio_by_name("gpio_plugdet");
	ctp_audio_pdata.codec_gpio_button = get_gpio_by_name("gpio_codec_int");
	ret = add_sst_platform_device();
	if (ret < 0)
		return NULL;

#ifdef CONFIG_JACK_MON
	pdev = platform_device_alloc("jack", -1);
	if (!pdev) {
		pr_err("failed to allocate jack platform device\n");
		return NULL;
	}

	ret = platform_device_add_data(pdev, &jack_data, sizeof(jack_data));
	if (ret) {
		pr_err("failed to add platform data to jack platform device\n");
		platform_device_put(pdev);
		return NULL;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		pr_err("failed to add jack platform device\n");
		platform_device_put(pdev);
		return NULL;
	}
#endif

	pdev = platform_device_alloc("compress-sst", -1);
	if (!pdev) {
		pr_err("failed to allocate compress-sst platform device\n");
		return NULL;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		pr_err("failed to add compress-sst platform device\n");
		platform_device_put(pdev);
		return NULL;
	}


	pdev = platform_device_alloc("hdmi-audio", -1);
	if (!pdev) {
		pr_err("failed to allocate hdmi-audio platform device\n");
		return NULL;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		pr_err("failed to add hdmi-audio platform device\n");
		platform_device_put(pdev);
		return NULL;
	}

	pdev = platform_device_alloc(pentry->name, -1);
	if (!pdev) {
		pr_err("failed to allocate clvcs_audio platform device\n");
		return NULL;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		pr_err("failed to add clvcs_audio platform device\n");
		platform_device_put(pdev);
		return NULL;
	}
	if (platform_device_add_data(pdev, &ctp_audio_pdata,
			sizeof(struct ctp_audio_platform_data))) {
		pr_err("failed to add ctp_audio platform data\n");
		platform_device_put(pdev);
		return NULL;
	}

	register_rpmsg_service("rpmsg_msic_clv_audio", RPROC_SCU,
				RP_MSIC_CLV_AUDIO);
	return NULL;
}
