#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/hdreg.h>
#include <linux/kdev_t.h>
#include <linux/blkdev.h>
#include <linux/mutex.h>

#include <linux/mmc/ioctl.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>

#include <linux/uaccess.h>

#include "queue.h"

//#include <linux/mmc/hw_write_protect.h>
#include <linux/scatterlist.h>

#include <linux/mmc/dsm_emmc.h>


unsigned int emmc_dsm_real_upload_size;
static unsigned int g_last_msg_code;	/*last sent error code*/
static unsigned int g_last_msg_count;	/*last sent the code count*/
#define ERR_MAX_COUNT 10

#define DSM_EMMC_IO_TIMEOUT_REPORT_INTERVAL	\
		((8 * 3600) * HZ)
static unsigned long g_emmc_io_timeout_event_count;

struct dsm_dev dsm_emmc = {
	.name = "dsm_emmc",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = EMMC_DSM_BUFFER_SIZE,
};
struct dsm_client *emmc_dclient;

/*the buffer which transffering to device radar*/
struct emmc_dsm_log g_emmc_dsm_log;

EXPORT_SYMBOL(emmc_dclient);

static void dsm_emmc_show_event_count(int code)
{
	unsigned long event_cnt = 0;

	switch (code) {
#ifndef CONFIG_HUAWEI_EMMC_DSM_ONLY_VDET
	case DSM_EMMC_IO_TIMEOUT:
		if (g_emmc_io_timeout_event_count)
			event_cnt = g_emmc_io_timeout_event_count;
		break;
#endif
	default:
		break;
	}

	if (event_cnt)
		pr_err("DSM_EMMC Err num: %d, count = %lu\n",
			code, event_cnt);
}

static int dsm_emmc_add_event_count(int code, char *buf, unsigned int cnt)
{
	int ret = 0;
	unsigned long event_cnt = 0;

	switch (code) {
#ifndef CONFIG_HUAWEI_EMMC_DSM_ONLY_VDET
	case DSM_EMMC_IO_TIMEOUT:
		if (g_emmc_io_timeout_event_count) {
			event_cnt = g_emmc_io_timeout_event_count;
			g_emmc_io_timeout_event_count = 0;
		}
		break;
#endif
	default:
		break;
	}

	if (event_cnt) {
		ret = snprintf(buf, cnt, "Event Count = %lu. ", event_cnt);
		if (ret < 0) {
			pr_err("%s:%d ++\n", __func__, __LINE__);
			ret = 0;
		}
	}

	return ret;
}

static int dsm_emmc_recaculate_is_report(int code, int old_ret)
{
	int new_ret = 0;
	static unsigned long emmc_io_timeout_next_report_time;

	switch (code) {
#ifndef CONFIG_HUAWEI_EMMC_DSM_ONLY_VDET
	case DSM_EMMC_IO_TIMEOUT:
		if (old_ret)
			emmc_io_timeout_next_report_time =
				jiffies + DSM_EMMC_IO_TIMEOUT_REPORT_INTERVAL;
		else
			g_emmc_io_timeout_event_count++;

		if (time_after(jiffies, emmc_io_timeout_next_report_time) &&
			!old_ret) {
			new_ret = 1;
			emmc_io_timeout_next_report_time =
				jiffies + DSM_EMMC_IO_TIMEOUT_REPORT_INTERVAL;
			if (g_emmc_io_timeout_event_count)
				g_emmc_io_timeout_event_count--;
		}
		break;
#endif
	default:
		new_ret = old_ret;
		break;
	}

	if (old_ret)
		return old_ret;

	return new_ret;
}

/*
 * first send the err msg to /dev/exception node.
 * if it produces lots of reduplicated msg, will just record the times
 * for every error, it's better to set max times
 * @code: error number
 * @err_msg: error message
 * @return: 0:don't report, 1: report
 */
static int dsm_emmc_process_log(int code, char *err_msg)
{
	int ret = 0;
	/*the MAX times of erevy err code*/
	static char vdet_err_max_count = ERR_MAX_COUNT;
#ifndef CONFIG_HUAWEI_EMMC_DSM_ONLY_VDET
	static char system_w_err_max_count = ERR_MAX_COUNT;
	static char erase_err_max_count = ERR_MAX_COUNT;
	static char send_cxd_err_max_count = ERR_MAX_COUNT;
	static char emmc_read_err_max_count = ERR_MAX_COUNT;
	static char emmc_write_err_max_count = ERR_MAX_COUNT;
	static char emmc_tuning_err_max_count = ERR_MAX_COUNT;
	static char emmc_set_width_err_max_count = ERR_MAX_COUNT;
	static char emmc_pre_eol_max_count = ERR_MAX_COUNT;
	static char emmc_life_time_err_max_count = ERR_MAX_COUNT;
	static char emmc_rsp_err_max_count = ERR_MAX_COUNT;
	static char emmc_rw_timeout_max_count = ERR_MAX_COUNT;
	static char emmc_host_err_max_count = ERR_MAX_COUNT;
	static char emmc_urgent_bkops_max_count = ERR_MAX_COUNT;
	static char emmc_dyncap_needed_max_count = ERR_MAX_COUNT;
	static char emmc_syspool_exhausted_max_count = ERR_MAX_COUNT;
	static char emmc_packed_failure_max_count = ERR_MAX_COUNT;
	static char emmc_io_timeout_max_count = ERR_MAX_COUNT;
#endif
	/*filter: if it has the same msg code with last, record err code&count*/
    if (g_last_msg_code == code) {
		ret = 0;
		g_last_msg_count++;
    } else {
		g_last_msg_code = code;
		g_last_msg_count = 0;
		ret = 1;
    }

	ret = dsm_emmc_recaculate_is_report(code, ret);

	/*restrict count of every error, note:deplicated msg donesn't its count*/
	if (1 == ret) {
		switch (code) {
		case DSM_EMMC_VDET_ERR:
			if (0 < vdet_err_max_count) {
				vdet_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
#ifndef CONFIG_HUAWEI_EMMC_DSM_ONLY_VDET
		case DSM_SYSTEM_W_ERR:
			if (0 < system_w_err_max_count) {
				system_w_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_ERASE_ERR:
			if (0 < erase_err_max_count) {
				erase_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_SEND_CXD_ERR:
			if (0 < send_cxd_err_max_count) {
				send_cxd_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_READ_ERR:
			if (0 < emmc_read_err_max_count) {
				emmc_read_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_WRITE_ERR:
			if (0 < emmc_write_err_max_count) {
				emmc_write_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_SET_BUS_WIDTH_ERR:
			if (0 < emmc_set_width_err_max_count) {
				emmc_set_width_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_PRE_EOL_INFO_ERR:
			if (0 < emmc_pre_eol_max_count) {
				emmc_pre_eol_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_TUNING_ERROR:
			if (0 < emmc_tuning_err_max_count) {
				emmc_tuning_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_LIFE_TIME_EST_ERR:
			if (0 < emmc_life_time_err_max_count) {
				emmc_life_time_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_RSP_ERR:
			if (0 < emmc_rsp_err_max_count) {
				emmc_rsp_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_RW_TIMEOUT_ERR:
			if (0 < emmc_rw_timeout_max_count) {
				emmc_rw_timeout_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_HOST_ERR:
			if (0 < emmc_host_err_max_count) {
				emmc_host_err_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_URGENT_BKOPS:
			if (0 < emmc_urgent_bkops_max_count) {
				emmc_urgent_bkops_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_DYNCAP_NEEDED:
			if (0 < emmc_dyncap_needed_max_count) {
				emmc_dyncap_needed_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_SYSPOOL_EXHAUSTED:
			if (0 < emmc_syspool_exhausted_max_count) {
				emmc_syspool_exhausted_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_PACKED_FAILURE:
			if (0 < emmc_packed_failure_max_count) {
				emmc_packed_failure_max_count--;
				ret = 1;
			} else {
				ret = 0;
			}
			break;
		case DSM_EMMC_IO_TIMEOUT:
			if(0 < emmc_io_timeout_max_count) {
				emmc_io_timeout_max_count--;
				ret = 1;
			}else {
				ret = 0;
			}
			break;
#endif
		default:
			ret = 0;
			break;
		}
	}

	return ret;
}

/*
 * Put error message into buffer.
 * @code: error number
 * @err_msg: error message
 * @return: 0:no buffer to report, 1: report
 */
int dsm_emmc_get_log(void *card, int code, char *err_msg)
{
	int ret = 0;
	int err = 0;
	unsigned int buff_size = sizeof(g_emmc_dsm_log.emmc_dsm_log);
	char *dsm_log_buff = g_emmc_dsm_log.emmc_dsm_log;
	struct mmc_card *card_dev = (struct mmc_card *)card;
	unsigned int last_msg_code = g_last_msg_code;
	unsigned int last_msg_count = g_last_msg_count;
	int i = 1;
	u8 *ext_csd = NULL;

	printk(KERN_ERR "DSM_EMMC enter dsm_emmc_get_log\n");
	if (dsm_emmc_process_log(code, err_msg)) {
		/*clear global buffer*/
		memset(g_emmc_dsm_log.emmc_dsm_log, 0, buff_size);

		/* print event count on expired event */
		ret = dsm_emmc_add_event_count(code, dsm_log_buff, buff_size);
		dsm_log_buff += ret;
		buff_size -= (unsigned int)ret;

		/*print duplicated code and its count */
		if ((0 < last_msg_count) && (0 == g_last_msg_count)) {
			ret = snprintf(dsm_log_buff, buff_size, "last Err num: %d, the times: %d\n",
				last_msg_code, last_msg_count + 1);
			if (ret < 0) {
				pr_err("%s:%d ++\n", __func__, __LINE__);
				ret = 0;
			}
			dsm_log_buff += ret;
			buff_size -= (unsigned int)ret;
			last_msg_code = 0;
			last_msg_count = 0;
		}

		printk(KERN_ERR "DSM_EMMC Err num: %d, %s\n", code, err_msg);
		/*print card CID info*/
		if (NULL != card_dev) {
			if (sizeof(struct mmc_cid) < buff_size) {
				ret = snprintf(dsm_log_buff, buff_size, "manufacturer_id:%04x Error Num:%lu, %s\n",
						card_dev->cid.manfid, code, err_msg);
				if (ret < 0) {
					pr_err("%s:%d ++\n", __func__, __LINE__);
					ret = 0;
				}
				dsm_log_buff += ret;
				buff_size -= (unsigned int)ret;

				ret = snprintf(dsm_log_buff, buff_size, "product_name:%s ",
						card_dev->cid.prod_name);
				if (ret < 0) {
					pr_err("%s:%d ++\n", __func__, __LINE__);
					ret = 0;
				}
				dsm_log_buff += ret;
				buff_size -= (unsigned int)ret;

				ret = snprintf(dsm_log_buff, buff_size, "fw:%02x%02x%02x%02x%02x%02x%02x%02x ", card_dev->ext_csd.fwrev[0],card_dev->ext_csd.fwrev[1],\
						card_dev->ext_csd.fwrev[2],card_dev->ext_csd.fwrev[3],card_dev->ext_csd.fwrev[4],card_dev->ext_csd.fwrev[5],\
						card_dev->ext_csd.fwrev[6],card_dev->ext_csd.fwrev[7]);

				if (ret < 0) {
					pr_err("%s:%d ++\n", __func__, __LINE__);
					ret = 0;
				}
				dsm_log_buff += ret;
				buff_size -= (unsigned int)ret;

				ret = snprintf(dsm_log_buff, buff_size, "manufacturer_date:%02x\n", (card_dev->raw_cid[3]>>8)&0xff);
				if (ret < 0) {
					pr_err("%s:%d ++\n", __func__, __LINE__);
					ret = 0;
				}
				dsm_log_buff += ret;
				buff_size -= (unsigned int)ret;

			} else {
				printk(KERN_ERR "%s:g_emmc_dsm_log Buff size is not enough\n", __FUNCTION__);
				printk(KERN_ERR "%s:eMMC error message is: %s\n", __FUNCTION__, err_msg);
			}

			/*print card ios info*/
			if (sizeof(card_dev->host->ios) < buff_size) {
				if (NULL != card_dev->host) {
					ret = snprintf(dsm_log_buff, buff_size,
						"Card's ios.clock:%uHz, ios.old_rate:%uHz, ios.power_mode:%u, ios.timing:%u, ios.bus_mode:%u, ios.bus_width:%u\n",
						card_dev->host->ios.clock, card_dev->host->ios.old_rate, card_dev->host->ios.power_mode, card_dev->host->ios.timing,
						card_dev->host->ios.bus_mode, card_dev->host->ios.bus_width);
					if (ret < 0) {
						pr_err("%s:%d ++\n", __func__, __LINE__);
						ret = 0;
					}
					dsm_log_buff += ret;
					buff_size -= (unsigned int)ret;
					pr_info("Card's ios.clock:%uHz, ios.old_rate:%uHz, ios.power_mode:%u, ios.timing:%u, ios.bus_mode:%u, ios.bus_width:%u\n",
						card_dev->host->ios.clock, card_dev->host->ios.old_rate, card_dev->host->ios.power_mode, card_dev->host->ios.timing,
						card_dev->host->ios.bus_mode, card_dev->host->ios.bus_width);
				}
			} else {
				printk(KERN_ERR "%s:g_emmc_dsm_log Buff size is not enough\n", __FUNCTION__);
				printk(KERN_ERR "%s:eMMC error message is: %s\n", __FUNCTION__, err_msg);
			}

			/*print card ext_csd info*/
			if (EMMC_EXT_CSD_LENGHT < buff_size) {

				if (NULL != card_dev->cached_ext_csd) {
					ret = snprintf(dsm_log_buff, buff_size,
						"eMMC ext_csd(Note: just static slice data is believable):\n");
					if (ret < 0) {
						pr_err("%s:%d ++\n", __func__, __LINE__);
						ret = 0;
					}
					dsm_log_buff += ret;
					buff_size -= (unsigned int)ret;

					ext_csd = card_dev->cached_ext_csd;
					for (i = 0; i < EMMC_EXT_CSD_LENGHT; i++) {
						ret = snprintf(dsm_log_buff, buff_size, "%02x", ext_csd[i]);
						if (ret < 0) {
							pr_err("%s:%d ++\n", __func__, __LINE__);
							ret = 0;
						}
						dsm_log_buff += ret;
						buff_size -= (unsigned int)ret;
					}
					ret = snprintf(dsm_log_buff, buff_size, "\n\n");
					if (ret < 0) {
						pr_err("%s:%d ++\n", __func__, __LINE__);
						ret = 0;
					}
					dsm_log_buff += ret;
					buff_size -= (unsigned int)ret;
				}
			} else {
				printk(KERN_ERR "%s:g_emmc_dsm_log Buff size is not enough\n", __FUNCTION__);
				printk(KERN_ERR "%s:eMMC error message is: %s\n", __FUNCTION__, err_msg);
			}
		}

		/*get size of used buffer*/
		emmc_dsm_real_upload_size = sizeof(g_emmc_dsm_log.emmc_dsm_log) - buff_size;
		pr_debug("DSM_DEBUG %s\n", g_emmc_dsm_log.emmc_dsm_log);
		printk(KERN_ERR "DSM_EMMC DSM_DEBUG %s\n", g_emmc_dsm_log.emmc_dsm_log);

		return 1;
	} else {
		printk("%s:Err num: %d, %s\n", __FUNCTION__, code, err_msg);
		dsm_emmc_show_event_count(code);
		if (NULL != card_dev) {
			pr_info("Card's ios.clock:%uHz, ios.old_rate:%uHz, ios.power_mode:%u,\
				ios.timing:%u, ios.bus_mode:%u, ios.bus_width:%u\n",
					card_dev->host->ios.clock, card_dev->host->ios.old_rate,
					card_dev->host->ios.power_mode, card_dev->host->ios.timing,
					card_dev->host->ios.bus_mode, card_dev->host->ios.bus_width);
		}
	}
	printk(KERN_ERR "DSM_EMMC leave dsm_emmc_get_log\n");
	return 0;
}
EXPORT_SYMBOL(dsm_emmc_get_log);

int dsm_emmc_get_life_time(struct mmc_card *card)
{
	int err = 0;

	/* eMMC v5.0 or later */
	if (!strncmp(mmc_hostname(card->host), "mmc0", sizeof("mmc0"))) {
		if (NULL == card->cached_ext_csd) {
			err = mmc_get_ext_csd(card, &card->cached_ext_csd);
			if (err) {
				pr_warn("%s: get ext csd failed err=%d \n",
					mmc_hostname(card->host), err);
				err = 0;
			}
		}
		if (card->ext_csd.rev >= 7) {
			err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_EXP_EVENTS_CTRL,
					EXT_CSD_DYNCAP_EVENT_EN,
					card->ext_csd.generic_cmd6_time);
			if (err) {
				pr_warn("%s: Enabling dyncap and syspool event failed\n",
					mmc_hostname(card->host));
				err = 0;
			}

			if (card->ext_csd.device_life_time_est_typ_a >= DEVICE_LIFE_TRIGGER_LEVEL ||
				card->ext_csd.device_life_time_est_typ_b >= DEVICE_LIFE_TRIGGER_LEVEL) {
					DSM_EMMC_LOG(card, DSM_EMMC_LIFE_TIME_EST_ERR,
						"%s:eMMC life time has problem, device_life_time_est_typ_a[268]:%d, device_life_time_est_typ_b{269]:%d\n",
						__FUNCTION__, card->ext_csd.device_life_time_est_typ_a, card->ext_csd.device_life_time_est_typ_b);
			}

			if (card->ext_csd.pre_eol_info == EXT_CSD_PRE_EOL_INFO_WARNING ||
				card->ext_csd.pre_eol_info == EXT_CSD_PRE_EOL_INFO_URGENT) {
				DSM_EMMC_LOG(card, DSM_EMMC_PRE_EOL_INFO_ERR,
					"%s:eMMC average reserved blocks has problem, PRE_EOL_INFO[267]:%d\n", __FUNCTION__,
					card->ext_csd.pre_eol_info);
			}
		}
	}

	return err;
}
EXPORT_SYMBOL(dsm_emmc_get_life_time);

void dsm_emmc_init(void)
{
	if (!emmc_dclient) {
		emmc_dclient = dsm_register_client(&dsm_emmc);
	}
	spin_lock_init(&g_emmc_dsm_log.lock);
}
EXPORT_SYMBOL(dsm_emmc_init);
