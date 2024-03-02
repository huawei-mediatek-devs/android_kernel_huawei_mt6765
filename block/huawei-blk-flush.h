/*
 * linux/block/huawei-blk-flush.c
 *
 * F2FS reduce flush function.
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

#ifndef INT_MTK_BLK_FLUSH_H
#define INT_MTK_BLK_FLUSH_H

extern void huawei_blk_queue_async_flush_init(struct request_queue *q);
extern bool huawei_blk_generic_make_request_check(struct bio *bio);
extern void huawei_blk_queue_register(struct request_queue *q,
	struct gendisk *disk);
extern ssize_t queue_var_store(unsigned long *var, const char *page,
	size_t count);
extern ssize_t huawei_queue_apd_tst_enable_store(struct request_queue *q,
	const char *page, size_t count);
extern void huawei_blk_allocated_queue_init(struct request_queue *q);
extern void huawei_blk_check_partition_done(struct gendisk *disk,
	bool has_part_tbl);

#endif
