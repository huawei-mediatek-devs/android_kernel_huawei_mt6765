/*
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

/*
 * Definitions for ALSPS als/ps sensor chip.
 */
#ifndef __ALSPSHUB_H__
#define __ALSPSHUB_H__

#include <linux/ioctl.h>


/*ALSPS related driver tag macro*/
#define ALSPS_SUCCESS						0
#define ALSPS_ERR_I2C						-1
#define ALSPS_ERR_STATUS					-3
#define ALSPS_ERR_SETUP_FAILURE				-4
#define ALSPS_ERR_GETGSENSORDATA			-5
#define ALSPS_ERR_IDENTIFICATION			-6

/*----------------------------------------------------------------------------*/
typedef enum {
	ALSPS_NOTIFY_PROXIMITY_CHANGE = 0,
} ALSPS_NOTIFY_TYPE;

enum ts_panel_id
{
    TS_PANEL_OFILIM 	 	= 0,
    TS_PANEL_EELY		 	= 1,
    TS_PANEL_TRULY	 		= 2,
    TS_PANEL_MUTTO	 		= 3,
    TS_PANEL_GIS		 	= 4,
    TS_PANEL_JUNDA	 		= 5,
    TS_PANEL_LENS 		 	= 6,
    TS_PANEL_YASSY	 		= 7,
    TS_PANEL_JDI 		 	= 8,
    TS_PANEL_SAMSUNG  		= 9,
    TS_PANEL_LG 		 	= 10,
    TS_PANEL_TIANMA 	 	= 11,
    TS_PANEL_CMI 		 	= 12,
    TS_PANEL_BOE  		 	= 13,
    TS_PANEL_CTC 		 	= 14,
    TS_PANEL_EDO 		 	= 15,
    TS_PANEL_SHARP	 		= 16,
    TS_PANEL_AUO 			= 17,
    TS_PANEL_TOPTOUCH 		= 18,
    TS_PANEL_BOE_BAK		= 19,
    TS_PANEL_CTC_BAK 		= 20,
    TS_PANEL_UNKNOWN 		= 0xFF,
};
#endif

