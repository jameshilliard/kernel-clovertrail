/*
 *  ctp_common.h - Common routines for the Clovertrail platform
 *
 *  Copyright (C) 2013 Intel Corp
 *  Author: Dharageswari.R <dharageswari.r@intel.com>
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#ifndef _CTP_COMMON_H
#define _CTP_COMMON_H

#include <linux/gpio.h>
#include <asm/intel-mid.h>

/* CDB42L73 Y1 (6.144 MHz) )oscillator =  MCLK1 */
#define DEFAULT_MCLK	19200000
#define SYSCLK_RATE	24576000

#define GPIO_AMP_ON 0x3d
#define GPIO_AMP_OFF 0x0
#define GPIOHVCTL 0x70

#define CTP_EXT_SPK_PA
#ifdef CTP_EXT_SPK_PA
#define EXT_PA_BOOST_EN_PIN	52
#define EXT_PA_EN_PIN	53
#define EXT_PA_ON 1
#define EXT_PA_OFF 0
#endif

enum {
	CTP_HSDET_GPIO = 0,
	CTP_BTN_GPIO,
};

struct comms_mc_private {
	bool ssp_bt_sco_master_mode;
	bool ssp_voip_master_mode;
	bool ssp_modem_master_mode;
	bool fm_state;
};

/* Headset jack detection gpios func(s) */
int ctp_soc_jack_gpio_detect(void);
int ctp_soc_jack_gpio_detect_bp(void);

extern struct snd_soc_machine_ops ctp_rhb_cs42l73_ops;
extern struct snd_soc_machine_ops ctp_vb_cs42l73_ops;

struct snd_soc_machine_ops {
	int micsdet_debounce;
	bool jack_support;
	int (*ctp_init)(struct snd_soc_pcm_runtime *runtime);
	int (*dai_link) (struct snd_soc_card *card);
	int (*hp_detection) (struct snd_soc_codec *codec,
			struct snd_soc_jack *jack, int plug_status);
	int (*bp_detection) (struct snd_soc_codec *codec,
			struct snd_soc_jack *jack, int plug_status);
	void (*mclk_switch) (struct device *dev, bool mode);
};

struct ctp_mc_private {
	struct comms_mc_private comms_ctl;
	struct ipc_device *socdev;
	void __iomem *int_base;
	struct snd_soc_codec *codec;
	/* Jack related */
	struct delayed_work jack_work_insert;
	struct delayed_work jack_work_remove;
	struct snd_soc_jack ctp_jack;
	struct snd_soc_jack_gpio *hs_gpio_ops;
	struct snd_soc_machine_ops *ops;
	atomic_t bpirq_flag;
	int bpirq;
	atomic_t hs_det_retry;
	bool btn_press_flag;
#ifdef CONFIG_HAS_WAKELOCK
	struct wake_lock *jack_wake_lock;
#endif
	bool voice_call_flag;
	bool headset_plug_flag;
};

static inline struct ctp_mc_private
*substream_to_drv_ctx(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *soc_card = rtd->card;
	return snd_soc_card_get_drvdata(soc_card);
}

static inline void ctp_config_voicecall_flag(
		struct snd_pcm_substream *substream, bool state)
{
	struct ctp_mc_private *ctx = substream_to_drv_ctx(substream);
	pr_debug("%s voice call flag: %d\n", __func__, state);
	ctx->voice_call_flag = state;
}

struct ctp_clk_fmt {
	int clk_id;
	int dir;
	unsigned int freq;
	unsigned int fmt;
};

int snd_ctp_init(struct snd_soc_pcm_runtime *runtime);
int ctp_init(struct snd_soc_pcm_runtime *runtime);
int ctp_vb_init(struct snd_soc_pcm_runtime *runtime);
int ctp_amp_event(struct snd_soc_dapm_widget *w, struct snd_kcontrol *k, int event);
int ctp_set_bias_level(struct snd_soc_card *card,
		struct snd_soc_dapm_context *dapm,
		enum snd_soc_bias_level level);
int ctp_set_bias_level_post(struct snd_soc_card *card,
		struct snd_soc_dapm_context *dapm,
		enum snd_soc_bias_level level);
#ifdef CTP_EXT_SPK_PA
int ext_pa_boost_get_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int ext_pa_boost_set_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int ext_pa_get_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int ext_pa_set_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
#endif
int ctp_set_clk_fmt(struct snd_soc_dai *codec_dai, struct ctp_clk_fmt *clk_fmt);


int get_fm_state(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol);
int set_fm_state(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol);

/* SoC card */
extern unsigned int rates_8000_16000[];

extern struct snd_pcm_hw_constraint_list constraints_8000_16000;

extern unsigned int rates_48000[];

extern struct snd_pcm_hw_constraint_list constraints_48000;

extern unsigned int rates_16000[];
extern struct snd_pcm_hw_constraint_list constraints_16000;
extern struct snd_soc_card snd_soc_card_ctp;
#endif
