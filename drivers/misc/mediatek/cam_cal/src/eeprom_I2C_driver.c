

#define PFX "CAM_CAL"
#define pr_fmt(fmt) PFX "[%s] " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/of.h>
#include "cam_cal.h"
#include "cam_cal_define.h"
#include "cam_cal_list.h"
#include <linux/dma-mapping.h>
#ifdef CONFIG_COMPAT
/* 64 bit */
#include <linux/fs.h>
#include <linux/compat.h>
#endif

unsigned int Common_read_region_hw_adapt(unsigned char *data, unsigned int size);

static DEFINE_SPINLOCK(g_spinLock);

/************************************************************
 * I2C read function (Common)
 ************************************************************/
static struct i2c_client *g_pstI2CclientG;

/* add for linux-4.4 */
#ifndef I2C_WR_FLAG
#define I2C_WR_FLAG (0x1000)
#define I2C_MASK_FLAG (0x00ff)
#endif

static int Read_I2C_CAM_CAL(u16 a_u2Addr, u32 ui4_length, u8 *a_puBuff)
{
	int i4RetValue = 0;
	char puReadCmd[2] = { (char)(a_u2Addr >> 8), (char)(a_u2Addr & 0xFF) };

	if (ui4_length > 8) {
		pr_debug("exceed I2c-mt65xx.c 8 bytes limitation\n");
		return -1;
	}
	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr =
		g_pstI2CclientG->addr & (I2C_MASK_FLAG | I2C_WR_FLAG);
	spin_unlock(&g_spinLock);

	i4RetValue = i2c_master_send(g_pstI2CclientG, puReadCmd, 2);
	if (i4RetValue != 2) {
		pr_debug("I2C send read address failed!!\n");
		return -1;
	}

	i4RetValue = i2c_master_recv(g_pstI2CclientG,
				     (char *)a_puBuff, ui4_length);
	if (i4RetValue != ui4_length) {
		pr_debug("I2C read data failed!!\n");
		return -1;
	}

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = g_pstI2CclientG->addr & I2C_MASK_FLAG;
	spin_unlock(&g_spinLock);
	return 0;
}

int iReadData_CAM_CAL(unsigned int ui4_offset,
		      unsigned int ui4_length, unsigned char *pinputdata)
{
	int i4RetValue = 0;
	int i4ResidueDataLength;
	u32 u4IncOffset = 0;
	u32 u4CurrentOffset;
	u8 *pBuff;

	i4ResidueDataLength = (int)ui4_length;
	u4CurrentOffset = ui4_offset;
	pBuff = pinputdata;
	do {
		if (i4ResidueDataLength >= 8) {
			i4RetValue = Read_I2C_CAM_CAL((u16)u4CurrentOffset, 8, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
			u4IncOffset += 8;
			i4ResidueDataLength -= 8;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
		} else {
			i4RetValue = Read_I2C_CAM_CAL((u16)u4CurrentOffset, i4ResidueDataLength, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
			u4IncOffset += 8;
			i4ResidueDataLength -= 8;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
			/* break; */
		}
	} while (i4ResidueDataLength > 0);

	return 0;
}

unsigned int Common_read_region(struct i2c_client *client, unsigned int addr,
				unsigned char *data, unsigned int size)
{
	g_pstI2CclientG = client;
	if (iReadData_CAM_CAL(addr, size, data) == 0)
		return size;
	else
		return 0;
}

/************************************************************
 * I2C read function (Custom)
 * Customer's driver can put on here
 * Below is an example
 ************************************************************/
#define PAGE_SIZE_ 256
static int iReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData,
		       u8 *a_pRecvData, u16 a_sizeRecvData, u16 i2cId)
{
	int i4RetValue = 0;

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = (i2cId >> 1);

	spin_unlock(&g_spinLock);
	i4RetValue = i2c_master_send(g_pstI2CclientG, a_pSendData, a_sizeSendData);
	if (i4RetValue != a_sizeSendData) {
		pr_debug("I2C send failed!!, Addr = 0x%x\n", a_pSendData[0]);
		return -1;
	}
	i4RetValue = i2c_master_recv(g_pstI2CclientG, (char *)a_pRecvData, a_sizeRecvData);
	if (i4RetValue != a_sizeRecvData) {
		pr_debug("I2C read failed!!\n");
		return -1;
	}
	return 0;
}

static int custom_read_region(u32 addr, u8 *data, u16 i2c_id, u32 size)
{
	u8 *buff = data;
	u32 size_to_read = size;

	int ret = 0;

	while (size_to_read > 0) {
		u8 page = addr / PAGE_SIZE_;
		u8 offset = addr % PAGE_SIZE_;
		char *Buff = data;

		if (iReadRegI2C(&offset, 1, (u8 *)Buff, 1, i2c_id + (page << 1)) < 0) {
			pr_debug("fail addr=0x%x 0x%x, P=%d, offset=0x%x",
				 addr, *Buff, page, offset);
			break;
		}
		addr++;
		buff++;
		size_to_read--;
		ret++;
	}
	pr_debug("addr =%x size %d data read = %d\n", addr, size, ret);
	return ret;
}
unsigned int Custom_read_region(struct i2c_client *client, unsigned int addr,
				unsigned char *data, unsigned int size)
{
	g_pstI2CclientG = client;
	if (custom_read_region(addr, data, g_pstI2CclientG->addr, size) == 0)
		return size;
	else
		return 0;
}
