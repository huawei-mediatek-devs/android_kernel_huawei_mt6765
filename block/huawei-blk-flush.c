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

#define pr_fmt(fmt) "[BLK-IO]" fmt
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/gfp.h>
#include <linux/blk-mq.h>
#include <trace/events/block.h>
#include "blk.h"
#include "blk-mq.h"
#include "huawei-blk-flush.h"

#define BLK_ASYNC_FLUSH_DELAY_MS	 20U

static LIST_HEAD(all_flush_q_list);
static DEFINE_MUTEX(all_flush_q_mutex);
static struct workqueue_struct *blk_async_flush_workqueue;

#define BLK_LLD_FLUSH_REDUCE_SUPPORT      (1ULL << 3)

static void huawei_blk_flush_work_fn(struct work_struct *work)
{
	struct blk_queue_cust *member = container_of(work,
		struct blk_queue_cust, flush_work.work);
	struct request_queue *q = container_of(member,
		struct request_queue, huawei_queue);
	struct block_device *bdev = bdget_disk(
		q->huawei_queue.queue_disk, 0);

	if (bdev != NULL) {
		int ret = blkdev_get(bdev, FMODE_WRITE, NULL);
		if (ret) {
			pr_err("<%s> blkdev_get fail! \r\n", __func__);
			return;
		}
		ret = blkdev_issue_flush(bdev, GFP_KERNEL, NULL);
		if (ret)
			pr_err("<%s> blkdev_issue_flush fail! \r\n", __func__);
		blkdev_put(bdev, FMODE_WRITE);
	}
}

/*
 * This function is used to judge if the flush should be dispatch sync
 * or not
 */
bool huawei_blk_flush_sync_dispatch(struct request_queue *q,
	struct bio *bio)
{
	if (bio == NULL)
		return false;
	if (unlikely(bio->bi_opf & REQ_PREFLUSH)) {
		if (unlikely((bio->huawei_bio.bi_async_flush == 1) &&
			(bio->bi_iter.bi_size == 0) &&
			q->huawei_queue.flush_optimise)) {
			cancel_delayed_work(&q->huawei_queue.flush_work);
			queue_delayed_work(blk_async_flush_workqueue,
				&q->huawei_queue.flush_work,
				msecs_to_jiffies(BLK_ASYNC_FLUSH_DELAY_MS));
			return false;
		}
		if (unlikely(!atomic_read(&q->huawei_queue.write_after_flush)))
			return false;

		atomic_set(&q->huawei_queue.write_after_flush, 0);
	}

	if (bio_op(bio) == REQ_OP_WRITE) {
		atomic_inc(&q->huawei_queue.write_after_flush);
		if (unlikely((bio->bi_opf & (REQ_PREFLUSH | REQ_FUA)) &&
			(!atomic_read(&q->huawei_queue.write_after_flush)))) {
			if (likely(bio->bi_iter.bi_size != 0))
				bio->bi_opf &= ~REQ_PREFLUSH;
		}
		return true;
	}

	return true;
}

/*
 * This interface will be called when a bio is submitted.
 */
bool huawei_blk_generic_make_request_check(struct bio *bio)
{
	struct request_queue *q = bdev_get_queue(bio->bi_bdev);

	if (unlikely(!huawei_blk_flush_sync_dispatch(q, bio))) {
		bio->bi_error = 0;
		bio_endio(bio);
		return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(huawei_blk_generic_make_request_check);

struct blk_dev_lld *huawei_blk_get_lld(struct request_queue *q)
{
	return q->queue_tags ? &q->queue_tags->lld_func : &q->lld_func;
}

static unsigned char blk_power_off_flush_executing;
/*
 * This function is used to send the emergency flush
 * before system power down
 */
void blk_power_off_flush(int emergency)
{
	struct blk_queue_cust *huawei_queue = NULL;
	struct request_queue *q = NULL;
	struct blk_dev_lld *blk_lld = NULL;

	if (blk_power_off_flush_executing)
		return;
	blk_power_off_flush_executing = 1;
	list_for_each_entry(huawei_queue, &all_flush_q_list, flush_queue_node) {
		if (huawei_queue->blk_part_tbl_exist) {
			q = (struct request_queue *)container_of(huawei_queue,
				struct request_queue, huawei_queue);
			blk_lld = huawei_blk_get_lld(q);
			if (blk_lld && blk_lld->blk_direct_flush_fn)
				blk_lld->blk_direct_flush_fn(q);
		}
	}
	blk_power_off_flush_executing = 0;
}
EXPORT_SYMBOL_GPL(blk_power_off_flush);

static void huawei_blk_flush_reduced_queue_register(
	struct request_queue *q)
{
	mutex_lock(&all_flush_q_mutex);
	list_add(&q->huawei_queue.flush_queue_node, &all_flush_q_list);
	mutex_unlock(&all_flush_q_mutex);
}

static void huawei_blk_flush_reduced_queue_unregister(
	struct request_queue *q)
{
	mutex_lock(&all_flush_q_mutex);
	list_del(&q->huawei_queue.flush_queue_node);
	mutex_unlock(&all_flush_q_mutex);
}

/*
 * This function is used to enable the register the emergency
 * flush interface to the request queue
 */
void blk_queue_direct_flush_register(struct request_queue *q,
	int (*blk_direct_flush_fn)(struct request_queue *))
{
	struct blk_dev_lld *blk_lld = huawei_blk_get_lld(q);

	if (q->huawei_queue.flush_queue_node.next == NULL &&
		q->huawei_queue.flush_queue_node.prev == NULL)
		INIT_LIST_HEAD(&q->huawei_queue.flush_queue_node);
	if (blk_direct_flush_fn != NULL) {
		blk_lld->blk_direct_flush_fn = blk_direct_flush_fn;
		huawei_blk_flush_reduced_queue_register(q);
	} else {
		blk_lld->blk_direct_flush_fn = NULL;
		huawei_blk_flush_reduced_queue_unregister(q);
	}
}
EXPORT_SYMBOL_GPL(blk_queue_direct_flush_register);

void huawei_blk_queue_async_flush_init(struct request_queue *q)
{
	struct blk_dev_lld *blk_lld = huawei_blk_get_lld(q);

	atomic_set(&q->huawei_queue.write_after_flush, 0);
	if (q->huawei_queue.flush_queue_node.next == NULL &&
		q->huawei_queue.flush_queue_node.prev == NULL)
		INIT_LIST_HEAD(&q->huawei_queue.flush_queue_node);

	INIT_DELAYED_WORK(&q->huawei_queue.flush_work,
		huawei_blk_flush_work_fn);

	if (blk_lld->blk_direct_flush_fn &&
		list_empty(&q->huawei_queue.flush_queue_node))
		huawei_blk_flush_reduced_queue_register(q);

	q->huawei_queue.flush_optimise = blk_lld->feature_flag &
		BLK_LLD_FLUSH_REDUCE_SUPPORT ? 1 : 0;

	if (blk_async_flush_workqueue == NULL)
		blk_async_flush_workqueue = alloc_workqueue(
			"async_flush", WQ_UNBOUND, 0);
}

/*
 * This function is used to allow the flush to dispatch async
 */
void blk_flush_set_async(struct bio *bio)
{
	bio->huawei_bio.bi_async_flush = 1;
}
EXPORT_SYMBOL_GPL(blk_flush_set_async);

/*
 * This function is used to enable the flush reduce on the request queue
 */
void blk_queue_flush_reduce_config(struct request_queue *q,
	bool flush_reduce_enable)
{
	struct blk_dev_lld *blk_lld = huawei_blk_get_lld(q);

	if (flush_reduce_enable)
		blk_lld->feature_flag |= BLK_LLD_FLUSH_REDUCE_SUPPORT;
	else
		blk_lld->feature_flag &= ~BLK_LLD_FLUSH_REDUCE_SUPPORT;
	q->huawei_queue.flush_optimise = flush_reduce_enable ? 1 : 0;
}
EXPORT_SYMBOL_GPL(blk_queue_flush_reduce_config);

/*
 * This function is for FS module to aware flush reduce function
 */
int blk_flush_async_support(struct block_device *bi_bdev)
{
	struct request_queue *q = bdev_get_queue(bi_bdev);

	return q->huawei_queue.flush_optimise;
}
EXPORT_SYMBOL_GPL(blk_flush_async_support);

/*
 * This function is for block driver module to aware flush reduce function
 */
int blk_queue_flush_async_support(struct request_queue *q)
{
	return q->huawei_queue.flush_optimise;
}
EXPORT_SYMBOL_GPL(blk_queue_flush_async_support);

/*
 * This interface will be called when request queue bind with block disk.
 */
void huawei_blk_queue_register(struct request_queue *q,
	struct gendisk *disk)
{
	q->huawei_queue.queue_disk = disk;
}
EXPORT_SYMBOL_GPL(huawei_blk_queue_register);

/*
 * This interface will be called after block device
 * partition table check done.
 */
void huawei_blk_check_partition_done(struct gendisk *disk,
	bool has_part_tbl)
{
	disk->queue->huawei_queue.blk_part_tbl_exist = has_part_tbl;
}

/*This interface will be called when init request queue.*/
void huawei_blk_allocated_queue_init(struct request_queue *q)
{
	q->huawei_queue.blk_part_tbl_exist = false;
	blk_power_off_flush_executing = 0;
}

/*Below is for Abnormal Power Down Test*/
unsigned long huawei_blk_ft_apd_enter(int mode)
{
	unsigned long irqflags = 0;

	switch (mode) {
	case 1:
		pr_err("%s: Disable interrupt \r\n", __func__);
		local_irq_save(irqflags);
		break;
	case 2:
		pr_err("%s: Disable preempt \r\n", __func__);
		preempt_disable();
		break;
	default:
		break;
	}
	return irqflags;
}

void huawei_blk_ft_apd_run(int index)
{
	switch (index) {
	case 0:
		blk_power_off_flush(0);
		break;
	case 1:
		pr_err("%s: Trigger Panic! \r\n", __func__);
		BUG();
		break;
	default:
		break;
	}
}

void huawei_blk_ft_apd_exit(int mode, unsigned long irqflags)
{
	switch (mode) {
	case 1:
		pr_err("%s: Enable interrupt \r\n", __func__);
		local_irq_restore(irqflags);
		break;
	case 2:
		pr_err("%s: Enable preempt \r\n", __func__);
		preempt_enable();
		break;
	default:
		break;
	}
}

ssize_t huawei_queue_apd_tst_enable_store(
	struct request_queue *q, const char *page, size_t count)
{
	ssize_t ret;
	unsigned long val;

	ret = queue_var_store(&val, page, count);
	if (ret >= 0) {
		unsigned long irqflags;
		int mode = val / 100;

		irqflags = huawei_blk_ft_apd_enter(mode);
		huawei_blk_ft_apd_run(val % 100);
		huawei_blk_ft_apd_exit(mode, irqflags);
	}
	return (ssize_t)count;
}
EXPORT_SYMBOL_GPL(huawei_queue_apd_tst_enable_store);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("debug for power module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
