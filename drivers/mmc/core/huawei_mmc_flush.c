/*
 * linux/drivers/mmc/core/huawei_mmc_flush.c
 *
 * Flush the cache to the non-volatile storage.
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/version.h>
#include "core.h"
#include "bus.h"
#include "host.h"
#include "mmc_ops.h"

#define MMC_TIMEOUT_INVALID 0xFFFFFFFF

/*
 * mmc_switch_irq_safe - mmc_switch func for mtk platform;
 * it can be called when the irq system cannot work.
 */
int mmc_switch_irq_safe(struct mmc_card *card)
{
	struct mmc_host *host = NULL;
	int err;
	struct mmc_command cmd = {0};
	struct mmc_request mrq = {NULL};

	WARN_ON(!card);
	WARN_ON(!card->host);
	host = card->host;
	cmd.opcode = MMC_SWITCH;
	cmd.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		(EXT_CSD_FLUSH_CACHE << 16) |
		(1 << 8) |
		EXT_CSD_CMD_SET_NORMAL;
	cmd.flags = MMC_CMD_AC;
	cmd.flags |= MMC_RSP_SPI_R1 | MMC_RSP_R1;
	cmd.data = NULL;
	cmd.busy_timeout = MMC_TIMEOUT_INVALID;
	mrq.cmd = &cmd;
	cmd.retries = 0;
	mrq.cmd->error = 0;
	mrq.cmd->mrq = &mrq;

	if (host->ops->send_cmd_direct) {
		/*this func will check busy after data send*/
		err = host->ops->send_cmd_direct(host, &mrq);
		if (err) {
			pr_err("%s: send_cmd_direct fail\n", __func__);
			return err;
		}
	}
	return 0;
}

/*
 * Flush the cache to the non-volatile storage.
 */
int mmc_flush_cache_direct(struct mmc_card *card)
{
	int err = 0;

	if (mmc_card_mmc(card) &&
		(card->ext_csd.cache_size > 0) &&
		(card->ext_csd.cache_ctrl & 1)) {
		err = mmc_switch_irq_safe(card);
		if (err)
			pr_err("%s: cache flush error %d\n",
					mmc_hostname(card->host), err);
		else
			pr_err("%s success!\n", __func__);
	}

	return err;
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("debug for power module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");

