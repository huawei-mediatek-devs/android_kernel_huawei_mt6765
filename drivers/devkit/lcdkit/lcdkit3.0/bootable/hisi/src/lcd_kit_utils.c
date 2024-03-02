#include "lcd_kit_disp.h"
#include "lcd_kit_power.h"
#include "lcd_kit_utils.h"
#include "lcd_kit_panels.h"

static void lcd_kit_orise2x(struct hisi_panel_info* pinfo)
{
	pinfo->ifbc_cmp_dat_rev0 = 1;
	pinfo->ifbc_cmp_dat_rev1 = 0;
	pinfo->ifbc_auto_sel = 0;
}

static void lcd_kit_vesa3x_single(struct hisi_panel_info* pinfo)
{
	/* dsc parameter info */
	pinfo->vesa_dsc.bits_per_component = 8;
	pinfo->vesa_dsc.bits_per_pixel = 8;
	pinfo->vesa_dsc.initial_xmit_delay = 512;
	pinfo->vesa_dsc.first_line_bpg_offset = 12;
	pinfo->vesa_dsc.mux_word_size = 48;
	/*    DSC_CTRL */
	pinfo->vesa_dsc.block_pred_enable = 1;
	pinfo->vesa_dsc.linebuf_depth = 9;
	/* RC_PARAM3 */
	pinfo->vesa_dsc.initial_offset = 6144;
	/* FLATNESS_QP_TH */
	pinfo->vesa_dsc.flatness_min_qp = 3;
	pinfo->vesa_dsc.flatness_max_qp = 12;
	/* DSC_PARAM4 */
	pinfo->vesa_dsc.rc_edge_factor = 0x6;
	pinfo->vesa_dsc.rc_model_size = 8192;
	/* DSC_RC_PARAM5: 0x330b0b */
	pinfo->vesa_dsc.rc_tgt_offset_lo = (0x330b0b >> 20) & 0xF;
	pinfo->vesa_dsc.rc_tgt_offset_hi = (0x330b0b >> 16) & 0xF;
	pinfo->vesa_dsc.rc_quant_incr_limit1 = (0x330b0b >> 8) & 0x1F;
	pinfo->vesa_dsc.rc_quant_incr_limit0 = (0x330b0b >> 0) & 0x1F;
	/* DSC_RC_BUF_THRESH0: 0xe1c2a38 */
	pinfo->vesa_dsc.rc_buf_thresh0 = (0xe1c2a38 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh1 = (0xe1c2a38 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh2 = (0xe1c2a38 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh3 = (0xe1c2a38 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH1: 0x46546269 */
	pinfo->vesa_dsc.rc_buf_thresh4 = (0x46546269 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh5 = (0x46546269 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh6 = (0x46546269 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh7 = (0x46546269 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH2: 0x7077797b */
	pinfo->vesa_dsc.rc_buf_thresh8 = (0x7077797b >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh9 = (0x7077797b >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh10 = (0x7077797b >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh11 = (0x7077797b >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH3: 0x7d7e0000 */
	pinfo->vesa_dsc.rc_buf_thresh12 = (0x7d7e0000 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh13 = (0x7d7e0000 >> 16) & 0xFF;
	/* DSC_RC_RANGE_PARAM0: 0x1020100 */
	pinfo->vesa_dsc.range_min_qp0 = (0x1020100 >> 27) & 0x1F; //lint !e572
	pinfo->vesa_dsc.range_max_qp0 = (0x1020100 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset0 = (0x1020100 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp1 = (0x1020100 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp1 = (0x1020100 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset1 = (0x1020100 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM1: 0x94009be */
	pinfo->vesa_dsc.range_min_qp2 = (0x94009be >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp2 = (0x94009be >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset2 = (0x94009be >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp3 = (0x94009be >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp3 = (0x94009be >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset3 = (0x94009be >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM2, 0x19fc19fa */
	pinfo->vesa_dsc.range_min_qp4 = (0x19fc19fa >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp4 = (0x19fc19fa >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset4 = (0x19fc19fa >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp5 = (0x19fc19fa >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp5 = (0x19fc19fa >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset5 = (0x19fc19fa >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM3, 0x19f81a38 */
	pinfo->vesa_dsc.range_min_qp6 = (0x19f81a38 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp6 = (0x19f81a38 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset6 = (0x19f81a38 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp7 = (0x19f81a38 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp7 = (0x19f81a38 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset7 = (0x19f81a38 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM4, 0x1a781ab6 */
	pinfo->vesa_dsc.range_min_qp8 = (0x1a781ab6 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp8 = (0x1a781ab6 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset8 = (0x1a781ab6 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp9 = (0x1a781ab6 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp9 = (0x1a781ab6 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset9 = (0x1a781ab6 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM5, 0x2af62b34 */
	pinfo->vesa_dsc.range_min_qp10 = (0x2af62b34 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp10 = (0x2af62b34 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset10 = (0x2af62b34 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp11 = (0x2af62b34 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp11 = (0x2af62b34 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset11 = (0x2af62b34 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM6, 0x2b743b74 */
	pinfo->vesa_dsc.range_min_qp12 = (0x2b743b74 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp12 = (0x2b743b74 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset12 = (0x2b743b74 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp13 = (0x2b743b74 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp13 = (0x2b743b74 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset13 = (0x2b743b74 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM7, 0x6bf40000 */
	pinfo->vesa_dsc.range_min_qp14 = (0x6bf40000 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp14 = (0x6bf40000 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset14 = (0x6bf40000 >> 16) & 0x3F;
}

static void lcd_kit_vesa3x_dual(struct hisi_panel_info* pinfo)
{
	pinfo->vesa_dsc.bits_per_component = 8;
	pinfo->vesa_dsc.linebuf_depth = 9;
	pinfo->vesa_dsc.bits_per_pixel = 8;
	pinfo->vesa_dsc.initial_xmit_delay = 512;
	pinfo->vesa_dsc.first_line_bpg_offset = 12;
	pinfo->vesa_dsc.mux_word_size = 48;
	/* DSC_CTRL */
	pinfo->vesa_dsc.block_pred_enable = 1;//0;
	/* RC_PARAM3 */
	pinfo->vesa_dsc.initial_offset = 6144;
	/* FLATNESS_QP_TH */
	pinfo->vesa_dsc.flatness_min_qp = 3;
	pinfo->vesa_dsc.flatness_max_qp = 12;
	/* DSC_PARAM4 */
	pinfo->vesa_dsc.rc_edge_factor = 0x6;
	pinfo->vesa_dsc.rc_model_size = 8192;
	/* DSC_RC_PARAM5: 0x330b0b */
	pinfo->vesa_dsc.rc_tgt_offset_lo = (0x330b0b >> 20) & 0xF;
	pinfo->vesa_dsc.rc_tgt_offset_hi = (0x330b0b >> 16) & 0xF;
	pinfo->vesa_dsc.rc_quant_incr_limit1 = (0x330b0b >> 8) & 0x1F;
	pinfo->vesa_dsc.rc_quant_incr_limit0 = (0x330b0b >> 0) & 0x1F;
	/* DSC_RC_BUF_THRESH0: 0xe1c2a38 */
	pinfo->vesa_dsc.rc_buf_thresh0 = (0xe1c2a38 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh1 = (0xe1c2a38 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh2 = (0xe1c2a38 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh3 = (0xe1c2a38 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH1: 0x46546269 */
	pinfo->vesa_dsc.rc_buf_thresh4 = (0x46546269 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh5 = (0x46546269 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh6 = (0x46546269 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh7 = (0x46546269 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH2: 0x7077797b */
	pinfo->vesa_dsc.rc_buf_thresh8 = (0x7077797b >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh9 = (0x7077797b >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh10 = (0x7077797b >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh11 = (0x7077797b >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH3: 0x7d7e0000 */
	pinfo->vesa_dsc.rc_buf_thresh12 = (0x7d7e0000 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh13 = (0x7d7e0000 >> 16) & 0xFF;
	/* DSC_RC_RANGE_PARAM0: 0x1020100 */
	pinfo->vesa_dsc.range_min_qp0 = (0x1020100 >> 27) & 0x1F; //lint !e572
	pinfo->vesa_dsc.range_max_qp0 = (0x1020100 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset0 = (0x1020100 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp1 = (0x1020100 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp1 = (0x1020100 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset1 = (0x1020100 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM1: 0x94009be */
	pinfo->vesa_dsc.range_min_qp2 = (0x94009be >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp2 = (0x94009be >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset2 = (0x94009be >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp3 = (0x94009be >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp3 = (0x94009be >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset3 = (0x94009be >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM2, 0x19fc19fa */
	pinfo->vesa_dsc.range_min_qp4 = (0x19fc19fa >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp4 = (0x19fc19fa >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset4 = (0x19fc19fa >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp5 = (0x19fc19fa >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp5 = (0x19fc19fa >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset5 = (0x19fc19fa >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM3, 0x19f81a38 */
	pinfo->vesa_dsc.range_min_qp6 = (0x19f81a38 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp6 = (0x19f81a38 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset6 = (0x19f81a38 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp7 = (0x19f81a38 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp7 = (0x19f81a38 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset7 = (0x19f81a38 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM4, 0x1a781ab6 */
	pinfo->vesa_dsc.range_min_qp8 = (0x1a781ab6 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp8 = (0x1a781ab6 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset8 = (0x1a781ab6 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp9 = (0x1a781ab6 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp9 = (0x1a781ab6 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset9 = (0x1a781ab6 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM5, 0x2af62b34 */
	pinfo->vesa_dsc.range_min_qp10 = (0x2af62b34 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp10 = (0x2af62b34 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset10 = (0x2af62b34 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp11 = (0x2af62b34 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp11 = (0x2af62b34 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset11 = (0x2af62b34 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM6, 0x2b743b74 */
	pinfo->vesa_dsc.range_min_qp12 = (0x2b743b74 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp12 = (0x2b743b74 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset12 = (0x2b743b74 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp13 = (0x2b743b74 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp13 = (0x2b743b74 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset13 = (0x2b743b74 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM7, 0x6bf40000 */
	pinfo->vesa_dsc.range_min_qp14 = (0x6bf40000 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp14 = (0x6bf40000 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset14 = (0x6bf40000 >> 16) & 0x3F;
}

static void lcd_kit_vesa3_75x_dual(struct hisi_panel_info* pinfo)
{
	pinfo->vesa_dsc.bits_per_component = 10;
	pinfo->vesa_dsc.linebuf_depth = 11;
	pinfo->vesa_dsc.bits_per_pixel = 8;
	pinfo->vesa_dsc.initial_xmit_delay = 512;
	pinfo->vesa_dsc.first_line_bpg_offset = 12;
	pinfo->vesa_dsc.mux_word_size = 48;
	/* DSC_CTRL */
	pinfo->vesa_dsc.block_pred_enable = 1;//0;
	/* RC_PARAM3 */
	pinfo->vesa_dsc.initial_offset = 6144;
	/* FLATNESS_QP_TH */
	pinfo->vesa_dsc.flatness_min_qp = 7;
	pinfo->vesa_dsc.flatness_max_qp = 16;
	/* DSC_PARAM4 */
	pinfo->vesa_dsc.rc_edge_factor = 0x6;
	pinfo->vesa_dsc.rc_model_size = 8192;
	/* DSC_RC_PARAM5: 0x330f0f */
	pinfo->vesa_dsc.rc_tgt_offset_lo = (0x330f0f >> 20) & 0xF;
	pinfo->vesa_dsc.rc_tgt_offset_hi = (0x330f0f >> 16) & 0xF;
	pinfo->vesa_dsc.rc_quant_incr_limit1 = (0x330f0f >> 8) & 0x1F;
	pinfo->vesa_dsc.rc_quant_incr_limit0 = (0x330f0f >> 0) & 0x1F;
	/* DSC_RC_BUF_THRESH0: 0xe1c2a38 */
	pinfo->vesa_dsc.rc_buf_thresh0 = (0xe1c2a38 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh1 = (0xe1c2a38 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh2 = (0xe1c2a38 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh3 = (0xe1c2a38 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH1: 0x46546269 */
	pinfo->vesa_dsc.rc_buf_thresh4 = (0x46546269 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh5 = (0x46546269 >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh6 = (0x46546269 >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh7 = (0x46546269 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH2: 0x7077797b */
	pinfo->vesa_dsc.rc_buf_thresh8 = (0x7077797b >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh9 = (0x7077797b >> 16) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh10 = (0x7077797b >> 8) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh11 = (0x7077797b >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH3: 0x7d7e0000 */
	pinfo->vesa_dsc.rc_buf_thresh12 = (0x7d7e0000 >> 24) & 0xFF;
	pinfo->vesa_dsc.rc_buf_thresh13 = (0x7d7e0000 >> 16) & 0xFF;
	/* DSC_RC_RANGE_PARAM0: 0x2022200 */
	pinfo->vesa_dsc.range_min_qp0 = (0x2022200 >> 27) & 0x1F; //lint !e572
	pinfo->vesa_dsc.range_max_qp0 = (0x2022200 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset0 = (0x2022200 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp1 = (0x2022200 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp1 = (0x2022200 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset1 = (0x2022200 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM1: 0x94009be */
	pinfo->vesa_dsc.range_min_qp2 = 5;//(0x94009be >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp2 = 9;//(0x94009be >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset2 = (0x94009be >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp3 = 5;//(0x94009be >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp3 = 10;//(0x94009be >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset3 = (0x94009be >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM2, 0x19fc19fa */
	pinfo->vesa_dsc.range_min_qp4 = 7;//(0x19fc19fa >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp4 = 11;//(0x19fc19fa >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset4 = (0x19fc19fa >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp5 = 7;//(0x19fc19fa >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp5 = 11;//(0x19fc19fa >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset5 = (0x19fc19fa >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM3, 0x19f81a38 */
	pinfo->vesa_dsc.range_min_qp6 = 7;//(0x19f81a38 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp6 = 11;//(0x19f81a38 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset6 = (0x19f81a38 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp7 = 7;//(0x19f81a38 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp7 = 12;//(0x19f81a38 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset7 = (0x19f81a38 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM4, 0x1a781ab6 */
	pinfo->vesa_dsc.range_min_qp8 = 7;//(0x1a781ab6 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp8 = 13;//(0x1a781ab6 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset8 = (0x1a781ab6 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp9 = 7;//(0x1a781ab6 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp9 = 14;//(0x1a781ab6 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset9 = (0x1a781ab6 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM5, 0x2af62b34 */
	pinfo->vesa_dsc.range_min_qp10 = 9;//(0x2af62b34 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp10 = 15;//(0x2af62b34 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset10 = (0x2af62b34 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp11 = 9;//(0x2af62b34 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp11 = 16;//(0x2af62b34 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset11 = (0x2af62b34 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM6, 0x2b743b74 */
	pinfo->vesa_dsc.range_min_qp12 = 9;//(0x2b743b74 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp12 = 17;//(0x2b743b74 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset12 = (0x2b743b74 >> 16) & 0x3F;
	pinfo->vesa_dsc.range_min_qp13 = 11;//(0x2b743b74 >> 11) & 0x1F;
	pinfo->vesa_dsc.range_max_qp13 = 17;//(0x2b743b74 >> 6) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset13 = (0x2b743b74 >> 0) & 0x3F;
	/* DSC_RC_RANGE_PARAM7, 0x6bf40000 */
	pinfo->vesa_dsc.range_min_qp14 = 17;//(0x6bf40000 >> 27) & 0x1F;
	pinfo->vesa_dsc.range_max_qp14 = 19;//(0x6bf40000 >> 22) & 0x1F;
	pinfo->vesa_dsc.range_bpg_offset14 = (0x6bf40000 >> 16) & 0x3F;
}

void lcd_kit_compress_config(int mode, struct hisi_panel_info* pinfo)
{
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return ;
	}
	switch (mode) {
		case IFBC_TYPE_ORISE2X:
			lcd_kit_orise2x(pinfo);
			break;
		case IFBC_TYPE_VESA3X_SINGLE:
			lcd_kit_vesa3x_single(pinfo);
			break;
		case IFBC_TYPE_VESA3_75X_DUAL:
			lcd_kit_vesa3_75x_dual(pinfo);
			break;
		case IFBC_TYPE_VESA3X_DUAL:
			lcd_kit_vesa3x_dual(pinfo);
			break;
		case IFBC_TYPE_NONE:
			break;
		default:
			LCD_KIT_ERR("not support compress mode:%d\n", mode);
			break;
	}
}

int lcd_kit_dsi_fifo_is_empty(uint32_t dsi_base)
{
    uint32_t pkg_status = 0;
    uint32_t phy_status = 0;
    int is_timeout = 1000;

    /*read status register*/
    do {
        pkg_status = inp32(dsi_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
        phy_status = inp32(dsi_base + MIPIDSI_PHY_STATUS_OFFSET);
        if ((pkg_status & 0x1) == 0x1 && !(phy_status & 0x2)){
            break;
        }
		mdelay(1);
    } while (is_timeout-- > 0);

    if (!is_timeout) {
        HISI_FB_ERR("mipi check empty fail: \n \
            MIPIDSI_CMD_PKT_STATUS = 0x%x \n \
            MIPIDSI_PHY_STATUS = 0x%x \n \
            MIPIDSI_INT_ST1_OFFSET = 0x%x \n",
            inp32(dsi_base + MIPIDSI_CMD_PKT_STATUS_OFFSET),
            inp32(dsi_base + MIPIDSI_PHY_STATUS_OFFSET),
            inp32(dsi_base + MIPIDSI_INT_ST1_OFFSET));
        return LCD_KIT_FAIL;
    }
    return LCD_KIT_OK;
}

int lcd_kit_dsi_fifo_is_full(uint32_t dsi_base)
{
	uint32_t pkg_status = 0;
	uint32_t phy_status = 0;
	int is_timeout = 1000;

	/*read status register*/
	do {
		pkg_status = inp32(dsi_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
		phy_status = inp32(dsi_base + MIPIDSI_PHY_STATUS_OFFSET);
		if ((pkg_status & 0x2) != 0x2 && !(phy_status & 0x2)) {
			break;
		}
		mdelay(1);
	} while (is_timeout-- > 0);

	if (!is_timeout) {
		LCD_KIT_ERR("mipi check full fail: \n \
						MIPIDSI_CMD_PKT_STATUS = 0x%x \n \
						MIPIDSI_PHY_STATUS = 0x%x \n \
						MIPIDSI_INT_ST1_OFFSET = 0x%x \n",
					inp32(dsi_base + MIPIDSI_CMD_PKT_STATUS_OFFSET),
					inp32(dsi_base + MIPIDSI_PHY_STATUS_OFFSET),
					inp32(dsi_base + MIPIDSI_INT_ST1_OFFSET));
		return LCD_KIT_FAIL;
	}
	return LCD_KIT_OK;
}

void lcd_kit_read_power_status(struct hisi_fb_data_type* hisifd)
{
	uint32_t status = 0;
	uint32_t try_times = 0;
    uint32_t status1 = 0;
    uint32_t try_times1 = 0;

	outp32(hisifd->mipi_dsi0_base + MIPIDSI_GEN_HDR_OFFSET, 0x0A06);
	status = inp32(hisifd->mipi_dsi0_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
	while (status & 0x10) {
		udelay(50);
		if (++try_times > 100) {
			try_times = 0;
			LCD_KIT_ERR("Read lcd power status timeout!\n");
			break;
		}

		status = inp32(hisifd->mipi_dsi0_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
	}
	status = inp32(hisifd->mipi_dsi0_base + MIPIDSI_GEN_PLD_DATA_OFFSET);
	LCD_KIT_INFO("Fastboot LCD Power State = 0x%x.\n", status);
    if (disp_info->dsi1_cmd_support){
            outp32(hisifd->mipi_dsi1_base + MIPIDSI_GEN_HDR_OFFSET, 0x0A06);
            status1 = inp32(hisifd->mipi_dsi1_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
            while (status1 & 0x10) {
                udelay(50);
                if (++try_times1 > 100) {
                    try_times1 = 0;
                    LCD_KIT_ERR("Read dsi1 lcd power status timeout!\n");
                    break;
                }
                status1 = inp32(hisifd->mipi_dsi1_base + MIPIDSI_CMD_PKT_STATUS_OFFSET);
            }
           status1 = inp32(hisifd->mipi_dsi1_base + MIPIDSI_GEN_PLD_DATA_OFFSET);
           LCD_KIT_INFO("Fastboot DSI1 LCD Power State = 0x%x.\n", status1);
      }
	return ;
}

int lcd_kit_pwm_set_backlight(struct hisi_fb_data_type *hisifd, uint32_t bl_level)
{
	return hisi_pwm_set_backlight(hisifd, bl_level);
}

int lcd_kit_sh_blpwm_set_backlight(struct hisi_fb_data_type *hisifd, uint32_t bl_level)
{
	return hisi_sh_blpwm_set_backlight(hisifd, bl_level);
}

int lcd_kit_get_tp_color(struct hisi_fb_data_type* hisifd)
{
	int ret = LCD_KIT_OK;
	uint8_t read_value[2] = {0};
	struct hisi_panel_info* pinfo = NULL;

	pinfo = hisifd->panel_info;
	if (disp_info->tp_color.support) {
		ret = lcd_kit_dsi_cmds_rx(hisifd, read_value, &disp_info->tp_color.cmds);
		if (ret) {
			pinfo->tp_color = 0;
		} else {
			pinfo->tp_color = read_value[0];
		}
		LCD_KIT_INFO("tp color = %d\n", pinfo->tp_color);
	} else {
		LCD_KIT_INFO("Not support tp color\n");
	}
	return ret;
}

int lcd_kit_get_elvss_info(struct hisi_fb_data_type* hisifd)
{
	int ret = LCD_KIT_OK;
	uint8_t read_value = 0;
	struct hisi_panel_info* pinfo = NULL;

	if(hisifd == NULL) {
		LCD_KIT_ERR("param hisifd is null, err\n");
		return LCD_KIT_FAIL;
	}
	pinfo = hisifd->panel_info;
	if(pinfo == NULL) {
		LCD_KIT_ERR("param pinfo is null, err\n");
		return LCD_KIT_FAIL;
	}
	pinfo->elvss_dim_val = LCD_KIT_ELVSS_DIM_DEFAULT;

	if (disp_info->elvss.support) {
		ret = lcd_kit_dsi_cmds_rx((void *)hisifd, &read_value, &disp_info->elvss.elvss_read_cmds);
		if(ret < 0) {
			LCD_KIT_ERR("elvss_read_cmds send err\n");
			return ret;
		}
		pinfo->elvss_dim_val = read_value;
		LCD_KIT_INFO("read in lcd_kit_get_elvss_info: 0x%x\n", pinfo->elvss_dim_val);

		if(disp_info->elvss.set_elvss_lp_support) {
			ret = lcd_kit_dsi_cmds_tx((void *)hisifd, &disp_info->elvss.elvss_prepare_cmds);
			if(ret < 0) {
				LCD_KIT_ERR("elvss_prepare_cmds send err\n");
				return ret;
			}
			disp_info->elvss.elvss_write_cmds.cmds[0].payload[1] = read_value | LCD_KIT_ELVSS_DIM_ENABLE_MASK;
			ret = lcd_kit_dsi_cmds_tx((void *)hisifd, &disp_info->elvss.elvss_write_cmds);
			if(ret < 0) {
				LCD_KIT_ERR("elvss_write_cmds send err\n");
				return ret;
			}
			ret = lcd_kit_dsi_cmds_tx((void *)hisifd, &disp_info->elvss.elvss_post_cmds);
			if(ret < 0) {
				LCD_KIT_ERR("elvss_post_cmds send err\n");
				return ret;
			}
		}
	} else {
		LCD_KIT_INFO("Not support elvss dim\n");
	}
	return ret;
}

int lcd_kit_panel_version_init(struct hisi_fb_data_type *hisifd)
{
	int ret;
	struct hisi_panel_info *pinfo = NULL;

	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo = hisifd->panel_info;
	if (pinfo == NULL) {
		LCD_KIT_ERR("param pinfo is null, err\n");
		return LCD_KIT_FAIL;
	}

	if (disp_info->panel_version.support) {
		if (disp_info->panel_version.enter_cmds.cmds != NULL) {
			ret = lcd_kit_dsi_cmds_tx(hisifd,
				&disp_info->panel_version.enter_cmds);
			if (ret) {
				LCD_KIT_ERR("tx cmd fail\n");
				return LCD_KIT_FAIL;
			}
		}

		ret = lcd_kit_dsi_cmds_rx(hisifd,
			(uint8_t *)disp_info->panel_version.read_value,
			&disp_info->panel_version.cmds);
		if (ret) {
			LCD_KIT_ERR("cmd fail\n");
			return LCD_KIT_FAIL;
		}
		lcd_panel_version_compare(hisifd);
	}
	else {
		LCD_KIT_INFO("panelVersion fail\n");
	}
	return LCD_KIT_FAIL;
}

int lcd_panel_version_compare(struct hisi_fb_data_type *hisifd)
{
	int i,j;
	struct hisi_panel_info *pinfo = NULL;

	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo = hisifd->panel_info;
	if (pinfo == NULL) {
		LCD_KIT_ERR("param pinfo is null, err\n");
		return LCD_KIT_FAIL;
	}

	for (i = 0; i < (int)disp_info->panel_version.version_number; i++) {
		for (j = 0; j < (int)disp_info->panel_version.value_number; j++) {
			if (disp_info->panel_version.read_value[j] !=
				disp_info->panel_version.value.arry_data[i].buf[j])
				break;

			if (j == ((int)disp_info->panel_version.value_number - 1)) {
				if (!strncmp(
					disp_info->panel_version.lcd_version_name[i], "V3",
					strlen(disp_info->panel_version.lcd_version_name[i])))
					pinfo->lcd_panel_version = VER_V3;
				else if (!strncmp(
					disp_info->panel_version.lcd_version_name[i], "V4",
					strlen(disp_info->panel_version.lcd_version_name[i])))
					pinfo->lcd_panel_version = VER_V4;
				else if (!strncmp(
					disp_info->panel_version.lcd_version_name[i], "VN1",
					strlen(disp_info->panel_version.lcd_version_name[i])))
					pinfo->lcd_panel_version = VER_VN1;
				else
					pinfo->lcd_panel_version = VER_VN2;
				LCD_KIT_INFO("Panel version is %d\n",
					pinfo->lcd_panel_version);
				return LCD_KIT_OK;
			}
		}
	}

	if (i == disp_info->panel_version.version_number) {
		LCD_KIT_INFO("panel_version not find\n");
		return LCD_KIT_FAIL;
	}

	return LCD_KIT_FAIL;
}

uint32_t lcd_kit_get_backlight_type(struct hisi_panel_info* pinfo)
{
	if (pinfo->bl_set_type & BL_SET_BY_PWM) {
		return BL_SET_BY_PWM;
	} else if (pinfo->bl_set_type & BL_SET_BY_BLPWM) {
		return BL_SET_BY_BLPWM;
	} else if (pinfo->bl_set_type & BL_SET_BY_SH_BLPWM) {
		return BL_SET_BY_SH_BLPWM;
	} else if (pinfo->bl_set_type & BL_SET_BY_MIPI) {
		return BL_SET_BY_MIPI;
	} else {
		return BL_SET_BY_NONE;
	}
}

uint32_t lcd_kit_blpwm_set_backlight(struct hisi_fb_data_type* hisifd, uint32_t bl_level)
{
	int ret = LCD_KIT_OK;
	static bool already_enable = FALSE;

	if(disp_info->rgbw.support) {
		disp_info->rgbw.backlight_cmds.cmds->payload[1]= (REG61H_VALUE_FOR_RGBW>>8)&0x0f;
		disp_info->rgbw.backlight_cmds.cmds->payload[2] = REG61H_VALUE_FOR_RGBW&0xff;
		ret = lcd_kit_dsi_cmds_tx(hisifd, &disp_info->rgbw.backlight_cmds);
		if (ret) {
			LCD_KIT_ERR("write backlight_cmds fail\n");
		}
	}
	ret = hisi_blpwm_set_bl(hisifd, bl_level);

	if(power_hdl->lcd_backlight.buf[0] == REGULATOR_MODE) {
		/*enable/disable backlight*/
		if (bl_level == 0 && already_enable) {
			lcd_kit_charger_ctrl(LCD_KIT_BL, DISABLE);
			already_enable = FALSE;
		} else if (!already_enable) {
			lcd_kit_charger_ctrl(LCD_KIT_BL, ENABLE);
			already_enable = TRUE;
		}
	}
	return ret;
}

int lcd_kit_set_mipi_backlight(struct hisi_fb_data_type* hisifd, uint32_t bl_level)
{
	static bool already_enable = FALSE;
	int ret = 0;

	if (common_ops->set_mipi_backlight) {
		common_ops->set_mipi_backlight(hisifd, bl_level);
	}
	if (power_hdl->lcd_backlight.buf[0] == REGULATOR_MODE) {
		if (bl_level == 0 && already_enable) {
			ret = lcd_kit_charger_ctrl(LCD_KIT_BL, DISABLE);
			already_enable = FALSE;
		} else if (!already_enable) {
			ret = lcd_kit_charger_ctrl(LCD_KIT_BL, ENABLE);
			already_enable = TRUE;
		}
	}
	return ret;
}

char *lcd_kit_get_default_compatible(uint32_t product_id, uint32_t lcd_id)
{
	uint32_t i = 0;
	char *product_name = NULL;

	/*get product name*/
	for (i = 0; i < ARRAY_SIZE(product_map); ++i) {
		if ((product_map[i].board_id == product_id)) {
			product_name = product_map[i].product_name;
			LCD_KIT_INFO("found product name:%s\n", product_name);
			break;
		}
	}
	/*if not found product name, use default compatible*/
	if (i >= ARRAY_SIZE(product_map)) {
		LCD_KIT_ERR("not found product name, use default compatible\n");
		return LCD_KIT_DEFAULT_COMPATIBLE;
	}
	/*find compatible from default panel*/
	for (i = 0; i < ARRAY_SIZE(default_panel_map); ++i) {
		if (!strncmp(product_name, default_panel_map[i].product_name, strlen(product_name)) &&
			(lcd_id == default_panel_map[i].gpio_id)) {
			LCD_KIT_INFO("default_panel_map[%d].compatible = %s\n", i, default_panel_map[i].compatible);
			return default_panel_map[i].compatible;
		}
	}
	LCD_KIT_ERR("not found compatible from default panel, use default compatible\n");
	return LCD_KIT_DEFAULT_COMPATIBLE;
}

char *lcd_kit_get_normal_compatible(uint32_t product_id, uint32_t lcd_id)
{
	int i = 0;

	/*find compatible from normal panel*/
	for (i = 0; i < ARRAY_SIZE(lcd_kit_map); ++i) {
		if ((lcd_kit_map[i].board_id == product_id) &&
			(lcd_kit_map[i].gpio_id == lcd_id))
			return lcd_kit_map[i].compatible;
	}
	return LCD_KIT_DEFAULT_COMPATIBLE;
}

char *lcd_kit_get_default_lcdname(uint32_t product_id, uint32_t lcd_id)
{
	uint32_t i = 0;
	char *product_name = NULL;

	/*get product name*/
	for (i = 0; i < ARRAY_SIZE(product_map); ++i) {
		if ((product_map[i].board_id == product_id)) {
			product_name = product_map[i].product_name;
			LCD_KIT_INFO("found product name:%s\n", product_name);
			break;
		}
	}
	/*if not found product name, use default compatible*/
	if (i >= ARRAY_SIZE(product_map)) {
		LCD_KIT_ERR("not found product name, use default lcd name\n");
		return LCD_KIT_DEFAULT_PANEL;
	}
	/*find lcd name from default panel*/
	for (i = 0; i < ARRAY_SIZE(default_panel_map); ++i) {
		if (!strncmp(product_name, default_panel_map[i].product_name, strlen(product_name)) &&
			(lcd_id == default_panel_map[i].gpio_id)) {
			LCD_KIT_INFO("default_panel_map[%d].lcd_name = %s\n", i, default_panel_map[i].lcd_name);
			return default_panel_map[i].lcd_name;
		}
	}
	LCD_KIT_ERR("not found lcd name from default panel, use default lcd name\n");
	return LCD_KIT_DEFAULT_PANEL;
}

char *lcd_kit_get_normal_lcdname(uint32_t product_id, uint32_t lcd_id)
{
	int i = 0;

	/*find compatible from normal panel*/
	for (i = 0; i < ARRAY_SIZE(lcd_kit_map); ++i) {
		if ((lcd_kit_map[i].board_id == product_id) &&
			(lcd_kit_map[i].gpio_id == lcd_id))
			return lcd_kit_map[i].lcd_name;
	}
	return LCD_KIT_DEFAULT_COMPATIBLE;
}


char* lcd_kit_get_compatible(uint32_t product_id, uint32_t lcd_id, int pin_num)
{
	if ((pin_num == LCD_1_PIN && lcd_id == DEF_LCD_1_PIN) ||
		(pin_num == LCD_2_PIN && lcd_id == DEF_LCD_2_PIN)) {
		/*not insert panel, use default panel*/
		return lcd_kit_get_default_compatible(product_id, lcd_id);
	} else {
		/*recognize insert panel*/
		return lcd_kit_get_normal_compatible(product_id, lcd_id);
	}
}

char* lcd_kit_get_lcd_name(uint32_t product_id, uint32_t lcd_id, int pin_num)
{
	if ((pin_num == LCD_1_PIN && lcd_id == DEF_LCD_1_PIN) ||
		(pin_num == LCD_2_PIN && lcd_id == DEF_LCD_2_PIN)) {
		/*not insert panel, use default panel*/
		return lcd_kit_get_default_lcdname(product_id, lcd_id);
	} else {
		/*recognize insert panel*/
		return lcd_kit_get_normal_lcdname(product_id, lcd_id);
	}
}

static int lcd_kit_parse_disp_info(char* compatible)
{
	char temp_name[VERSION_NUM_MAX] = {0};
	char *name[VERSION_NUM_MAX] = {NULL};
	char *temp = NULL;
	int i;
	u32 temp_fp_prepare_support = 0;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops) {
		LCD_KIT_ERR("get adapt_ops error\n");
		return LCD_KIT_FAIL;
	}

	/*quickly sleep out*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,quickly-sleep-out-support", &disp_info->quickly_sleep_out.support, 0);
	if (disp_info->quickly_sleep_out.support) {
		lcd_kit_parse_get_u32_default(compatible, "lcd-kit,quickly-sleep-out-interval", &disp_info->quickly_sleep_out.interval, 0);
	}
	/*tp color support*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,tp-color-support", &disp_info->tp_color.support, 0);
	if (disp_info->tp_color.support) {
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,tp-color-cmds", "lcd-kit,tp-color-cmds-state", &disp_info->tp_color.cmds);
	}
	/*dsi1 support*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,dsi1-support", &disp_info->dsi1_cmd_support, 0);
	/*max brightness and color coordinate support*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,brightness-color-uniform-support", &disp_info->brightness_color_uniform.support, 0);
	if (disp_info->brightness_color_uniform.support) {
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,module-sn-cmds", "lcd-kit,module-sn-cmds-state", &disp_info->brightness_color_uniform.modulesn_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,equip-id-cmds", "lcd-kit,equip-id-cmds-state", &disp_info->brightness_color_uniform.equipid_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,module-manufact-cmds", "lcd-kit,module-manufact-cmds-state", &disp_info->brightness_color_uniform.modulemanufact_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,vendor-id-cmds", "lcd-kit,vendor-id-cmds-state", &disp_info->brightness_color_uniform.vendorid_cmds);
	}
	/*parse aod param*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,sensorhub-aod-support", &disp_info->aod.support, 0);
	if (disp_info->aod.support) {
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-enter-aod-cmds", "lcd-kit,panel-aod-on-cmds-state", &(disp_info->aod.aod_on_cmds));
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-exit-aod-cmds", "lcd-kit,panel-aod-off-cmds-state", &(disp_info->aod.aod_off_cmds));
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-aod-high-brightness-cmds", "lcd-kit,panel-aod-high-brightness-cmds-state", &(disp_info->aod.aod_high_brightness_cmds));
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-aod-low-brightness-cmds", "lcd-kit,panel-aod-low-brightness-cmds-state", &(disp_info->aod.aod_low_brightness_cmds));
	}

	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,hbm-elvss-dim-support", &disp_info->elvss.support, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,hbm-set-elvss-dim-lp", &disp_info->elvss.set_elvss_lp_support, 0);
	lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-hbm-elvss-prepare-cmds", "lcd-kit,panel-hbm-elvss-prepare-cmds-state", &(disp_info->elvss.elvss_prepare_cmds));
	lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-hbm-elvss-read-cmds", "lcd-kit,panel-hbm-elvss-read-cmds-state", &(disp_info->elvss.elvss_read_cmds));
	lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-hbm-elvss-write-cmds", "lcd-kit,panel-hbm-elvss-write-cmds-state", &(disp_info->elvss.elvss_write_cmds));
	lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,panel-hbm-elvss-post-cmds", "lcd-kit,panel-hbm-elvss-post-cmds-state", &(disp_info->elvss.elvss_post_cmds));
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,hbm-fp-prepare-support",
		&temp_fp_prepare_support, 0);
	disp_info->pwm.support = (u8)temp_fp_prepare_support;
	if (disp_info->pwm.support) {
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-hbm-fp-prepare-cmds",
			"lcd-kit,panel-hbm-fp-prepare-cmds-state",
			&(disp_info->pwm.hbm_fp_prepare_cmds));
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-hbm-fp-post-cmds",
			"lcd-kit,panel-hbm-fp-post-cmds-state",
			&(disp_info->pwm.hbm_fp_post_cmds));
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-hbm-fp-prepare-cmds-vn1",
			"lcd-kit,panel-hbm-fp-prepare-cmds-vn1-state",
			&(disp_info->pwm.hbm_fp_prepare_cmds));
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-hbm-fp-post-cmds-vn1",
			"lcd-kit,panel-hbm-fp-post-cmds-vn1-state",
			&(disp_info->pwm.hbm_fp_post_cmds));
	}

	/*panel version*/
	lcd_kit_parse_get_u32_default(compatible,
		"lcd-kit,panel-version-support",
		&disp_info->panel_version.support, 0);
	if (disp_info->panel_version.support) {
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-version-enter-cmds",
			"lcd-kit,panel-version-enter-cmds-state",
			&disp_info->panel_version.enter_cmds);
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,panel-version-cmds",
			"lcd-kit,panel-version-cmds-state",
			&disp_info->panel_version.cmds);
		disp_info->panel_version.value_number =
			disp_info->panel_version.cmds.cmds->dlen -
			disp_info->panel_version.cmds.cmds->payload[1];
		lcd_kit_parse_arrays(compatible,
			"lcd-kit,panel-version-value",
			&disp_info->panel_version.value,
			disp_info->panel_version.value_number);
		disp_info->panel_version.version_number =
			disp_info->panel_version.value.cnt;

		LCD_KIT_INFO("value_number = %d version_number = %d\n",
			disp_info->panel_version.value_number,
			disp_info->panel_version.version_number);
		if (disp_info->panel_version.version_number > 0) {
			if (adapt_ops->get_string_by_property) {
				adapt_ops->get_string_by_property(compatible,
					"lcd-kit,panel-fastboot-version",
					temp_name, VERSION_NUM_MAX);
				name[0] = strtok_s(temp_name, ",", &temp);
				for (i = 1;
					(i < (int)disp_info->panel_version.version_number) &&
					(name[i-1] != NULL); i++) {
					name[i] = strtok_s(NULL, ",", &temp);
					strncpy(disp_info->panel_version.lcd_version_name[i],
						name[i], LCD_PANEL_VERSION_SIZE - 1);
					LCD_KIT_INFO("lcd_version_name[%d] = %s\n", i, name[i]);
				}
			}
		}
	}

	lcd_kit_parse_get_u32_default(compatible,
		"lcd-kit,hbm-special-bit-ctrl-support",
		&disp_info->hbm.hbm_special_bit_ctrl, 0);
	/* hbm */
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,hbm-fp-support",
		&disp_info->hbm.support, 0);
	if (disp_info->hbm.support) {
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-enter-cmds",
			"lcd-kit,hbm-enter-cmds-state",
			&disp_info->hbm.enter_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-prepare-cmds",
			"lcd-kit,hbm-prepare-cmds-state",
			&disp_info->hbm.hbm_prepare_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-cmds",
			"lcd-kit,hbm-cmds-state",
			&disp_info->hbm.hbm_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-post-cmds",
			"lcd-kit,hbm-post-cmds-state",
			&disp_info->hbm.hbm_post_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,enter-dim-cmds",
			"lcd-kit,enter-dim-cmds-state",
			&disp_info->hbm.enter_dim_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,exit-dim-cmds",
			"lcd-kit,exit-dim-cmds-state",
			&disp_info->hbm.exit_dim_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-exit-cmds",
			"lcd-kit,hbm-exit-cmds-state",
			&disp_info->hbm.exit_cmds);
		lcd_kit_parse_dcs_cmds(compatible, "lcd-kit,hbm-fp-enter-cmds",
			"lcd-kit,hbm-fp-enter-cmds-state",
			&disp_info->hbm.fp_enter_cmds);
	}

	/*logo checksum*/
	lcd_kit_parse_get_u32_default(compatible,
		"lcd-kit,logo-checksum-support",
		&disp_info->logo_checksum.support, 0);
	if (disp_info->logo_checksum.support) {
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,checksum-enable-cmds",
			"lcd-kit,checksum-enable-cmds-state",
			&disp_info->logo_checksum.enable_cmds);
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,checksum-disable-cmds",
			"lcd-kit,checksum-disable-cmds-state",
			&disp_info->logo_checksum.disable_cmds);
		lcd_kit_parse_dcs_cmds(compatible,
			"lcd-kit,checksum-cmds",
			"lcd-kit,checksum-cmds-state",
			&disp_info->logo_checksum.checksum_cmds);
		lcd_kit_parse_array(compatible,
			"lcd-kit,logo-checksum-value",
			&disp_info->logo_checksum.value);
	}

	return LCD_KIT_OK;
}

static int lcd_kit_vesa_para_parse(char* compatible, struct hisi_panel_info* pinfo)
{
	if (!compatible) {
		LCD_KIT_ERR("compatible is null\n");
		return LCD_KIT_FAIL;
	}
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,vesa-slice-width", &pinfo->vesa_dsc.slice_width, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,vesa-slice-height", &pinfo->vesa_dsc.slice_height, 0);
	return LCD_KIT_OK;
}

void lcd_kit_set_mipi_tx_link(struct hisi_fb_data_type *hisifd, int link_state)
{
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return ;
	}

	/*wait fifo cmd empty*/
	lcd_kit_dsi_fifo_is_empty(hisifd->mipi_dsi0_base);
	LCD_KIT_INFO("link_state:%d\n", link_state);
	switch (link_state) {
		case LCD_KIT_DSI_LP_MODE:
			if (is_mipi_cmd_panel(hisifd)) {
				/*gen short cmd write switch low-power,include 0-parameter,1-parameter,2-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x7, 3, 8);
				/*gen long cmd write switch low-power*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x1, 1, 14);
				/*dcs short cmd write switch low-power,include 0-parameter,1-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x3, 2, 16);
			}
			break;
		case LCD_KIT_DSI_HS_MODE:
			if (is_mipi_cmd_panel(hisifd)) {
				/*gen short cmd write switch high-speed,include 0-parameter,1-parameter,2-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 3, 8);
				/*gen long cmd write switch high-speed*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 1, 14);
				/*dcs short cmd write switch high-speed,include 0-parameter,1-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 2, 16);
			}
			break;
		default:
			LCD_KIT_ERR("not support mode\n");
			break;
	}
}

void lcd_kit_set_mipi_rx_link(struct hisi_fb_data_type *hisifd, int link_state)
{
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return ;
	}
	/*wait fifo cmd empty*/
	lcd_kit_dsi_fifo_is_empty(hisifd->mipi_dsi0_base);
	LCD_KIT_INFO("link_state:%d\n", link_state);
	switch (link_state) {
		case LCD_KIT_DSI_LP_MODE:
			if (is_mipi_cmd_panel(hisifd)) {
				/*gen short cmd read switch low-power,include 0-parameter,1-parameter,2-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x7, 3, 11);
				/*dcs short cmd read switch low-power*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x1, 1, 18);
				/*read packet size cmd switch low-power*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x1, 1, 24);
			}
			break;
		case LCD_KIT_DSI_HS_MODE:
			if (is_mipi_cmd_panel(hisifd)) {
				/*gen short cmd read switch high-speed,include 0-parameter,1-parameter,2-parameter*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 3, 11);
				/*dcs short cmd read switch high-speed*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 1, 18);
				/*read packet size cmd switch high-speed*/
				set_reg(hisifd->mipi_dsi0_base + MIPIDSI_CMD_MODE_CFG_OFFSET, 0x0, 1, 24);
			}
			break;
		default:
			LCD_KIT_ERR("not support mode\n");
			break;
	}
}

int lcd_kit_change_dts_value(char *compatible, char *dts_name, u32 value)
{
	struct fdt_operators *fdt_ops = NULL;
	struct dtb_operators *dtimage_ops = NULL;
	struct fdt_property *property = NULL;
	void *fdt = NULL;
	int ret = 0;
	int offset = 0;
	int len = 0;
	uint32_t temp;

	if (!compatible || !dts_name) {
		LCD_KIT_ERR("null pointer found!\n");
		return LCD_KIT_FAIL;
	}

	fdt_ops = get_operators(FDT_MODULE_NAME_STR);
	if(!fdt_ops){
		LCD_KIT_ERR("can not get fdt_ops!\n");
		return LCD_KIT_FAIL;
	}

	dtimage_ops = get_operators(DTIMAGE_MODULE_NAME_STR);
	if (NULL == dtimage_ops) {
		LCD_KIT_ERR("failed to get dtimage operators!\n");
		return LCD_KIT_FAIL;
	}
	fdt_ops->fdt_operate_lock();
	fdt = dtimage_ops->get_dtb_addr();
	if (NULL == fdt) {
		LCD_KIT_ERR("failed to get fdt addr!\n");
		fdt_ops->fdt_operate_unlock();
		return LCD_KIT_FAIL;
	}

	ret = fdt_ops->fdt_open_into(fdt, fdt, DTS_SPACE_LEN);
	if (ret < 0) {
		LCD_KIT_ERR("fdt_open_into failed!\n");
		fdt_ops->fdt_operate_unlock();
		return LCD_KIT_FAIL;
	}

	ret = fdt_ops->fdt_node_offset_by_compatible(fdt, 0, compatible);
	if (ret < 0) {
		LCD_KIT_ERR("Could not find node:%s, change fb dts failed\n", compatible);
		fdt_ops->fdt_operate_unlock();
		return LCD_KIT_FAIL;
	}

	offset = ret;
	property = (struct fdt_property *)fdt_ops->fdt_get_property(fdt, offset, dts_name, &len);
	if(!property){
		LCD_KIT_ERR("-----can not find %s \n", dts_name);
		fdt_ops->fdt_operate_unlock();
		return LCD_KIT_FAIL;
	}
	if (!property->data) {
		fdt_ops->fdt_operate_unlock();
		LCD_KIT_ERR("property->data is null!\n");
		return LCD_KIT_FAIL;
	}
	temp = *property->data;
	temp = (uint32_t)fdt32_to_cpu(temp);
	ret = fdt_ops->fdt_setprop_u32(fdt, offset,
		(const char *)dts_name, (value | temp));
	if (ret) {
		LCD_KIT_ERR("Cannot update dts feild:%s errno=%d!\n", dts_name, ret);
		fdt_ops->fdt_operate_unlock();
		return LCD_KIT_FAIL;
	}
	fdt_ops->fdt_operate_unlock();
	return LCD_KIT_OK;
}

static void lcd_kit_aod_cmd_init(struct dsi_cmd_desc ** pdst_cmds,
								struct lcd_kit_dsi_panel_cmds * psrc_cmds)
{
	uint8_t index = 0;
	uint8_t j     = 0;
	struct dsi_cmd_desc * temp_cmds = NULL;

	if ((!psrc_cmds)||(!pdst_cmds)) {
		LCD_KIT_ERR("psrc_cmds is null\n");
		return ;
	}

	LCD_KIT_INFO("cmd_cnt is %d\n", psrc_cmds->cmd_cnt);
	if((0 == psrc_cmds->cmd_cnt)||(100 < psrc_cmds->cmd_cnt)) {
		LCD_KIT_ERR("cmd_cnt is invalid.\n");
		return;
	}

	if(NULL == psrc_cmds->cmds) {
		LCD_KIT_ERR("psrc_cmds->cmds is null\n");
		return ;
	}
	temp_cmds = alloc(psrc_cmds->cmd_cnt * sizeof(struct dsi_cmd_desc));
	if (!temp_cmds) {
		LCD_KIT_ERR("alloc fail.\n");
		return;
	}
	for(index = 0; index < psrc_cmds->cmd_cnt; index ++) {
		temp_cmds[index].dtype     = psrc_cmds->cmds[index].dtype;
		temp_cmds[index].vc        = psrc_cmds->cmds[index].vc;
		temp_cmds[index].wait      = psrc_cmds->cmds[index].wait;
		temp_cmds[index].waittype  = psrc_cmds->cmds[index].waittype;
		temp_cmds[index].dlen      = psrc_cmds->cmds[index].dlen;
		temp_cmds[index].payload   = alloc(psrc_cmds->cmds[index].dlen * sizeof(char));
		if(!(temp_cmds[index].payload)) {
			continue;
		}
		for (j = 0; j < (psrc_cmds->cmds[index].dlen); j++) {
			temp_cmds[index].payload[j] = psrc_cmds->cmds[index].payload[j];
		}
	}
	*pdst_cmds = temp_cmds;
}

/*****************************************************
 * Initialize related parameters sent to sensorhub   *
 ****************************************************/
static void lcd_kit_aod_init(struct hisi_panel_info* pinfo )
{
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return ;
	}

	/* discharge gpio */
	if (power_hdl->lcd_aod.buf != NULL)
		pinfo->dis_gpio = power_hdl->lcd_aod.buf[1];

	/*normal on*/
	pinfo->disp_on_cmds_len = common_info->panel_on_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->disp_on_cmds), &(common_info->panel_on_cmds));

	/*normal off*/
	pinfo->disp_off_cmds_len = common_info->panel_off_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->disp_off_cmds), &(common_info->panel_off_cmds));

	/*aod on*/
	pinfo->aod_enter_cmds_len = disp_info->aod.aod_on_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->aod_enter_cmds), &(disp_info->aod.aod_on_cmds));

	/*aod off*/
	pinfo->aod_exit_cmds_len = disp_info->aod.aod_off_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->aod_exit_cmds), &(disp_info->aod.aod_off_cmds));

	/*gpio reset*/
	static uint32_t gpio_reset = 0;
	if(NULL != power_hdl->lcd_rst.buf) {
		gpio_reset = power_hdl->lcd_rst.buf[1];
	}
	static struct gpio_desc gpio_reset_request_cmds[] = {
		{0, 1, 0, "gpio_lcdkit_reset", &gpio_reset, 0 },
	};
	pinfo->gpio_request_cmds = gpio_reset_request_cmds;
	pinfo->gpio_request_cmds_len = ARRAY_SIZE(gpio_reset_request_cmds);

	/*pinctrl normal*/
	static uint32_t gpio_te0 = 0;
	if(NULL != power_hdl->lcd_te0.buf) {
		gpio_te0 = power_hdl->lcd_te0.buf[1];
	}

	static struct gpio_desc pinctrl_normal_cmds[] = {
		/* reset */
		{ 4, 1, 0, "gpio_lcdkit_reset", &gpio_reset, FUNCTION0 },
		/* te0 */
		{ 4, 1, 0, "gpio_lcdkit_te0", &gpio_te0, FUNCTION2 },
	};
	pinfo->pinctrl_normal_cmds_len = ARRAY_SIZE(pinctrl_normal_cmds);
	pinfo->pinctrl_normal_cmds = pinctrl_normal_cmds;

	/*pinctrl lowpower*/
	static struct gpio_desc pinctrl_low_cmds[] = {
		/* reset */
		{ 4, 1, 0, "gpio_lcdkit_reset", &gpio_reset, FUNCTION0 },
		/* te0 */
		{4, 1, 0, "gpio_lcdkit_te0", &gpio_te0, FUNCTION0 },
		/* te0 input */
		{ 2, 1, 0, "gpio_lcdkit_te0", &gpio_te0, 0 },
	};
	pinfo->pinctrl_lowpower_cmds_len = ARRAY_SIZE(pinctrl_low_cmds);
	pinfo->pinctrl_lowpower_cmds = pinctrl_low_cmds;

	/*gpio reset normal*/
	static struct gpio_desc gpio_reset_normal_cmds[] = {
		/* reset */
		{ 3, 1, 15,"gpio_lcdkit_reset", &gpio_reset, 1 },
		{ 3, 1, 5,"gpio_lcdkit_reset", &gpio_reset, 0 },
		{ 3, 1, 15,"gpio_lcdkit_reset", &gpio_reset, 1 },
	};
	pinfo->gpio_normal_cmds1_len = ARRAY_SIZE(gpio_reset_normal_cmds);
	pinfo->gpio_normal_cmds1 = gpio_reset_normal_cmds;

	/*gpio reset low*/
	static struct gpio_desc gpio_reset_low_cmds[] = {
		/* reset */
		{ 3, 1, 5,"gpio_lcdkit_reset", &gpio_reset, 0 },
	};
	pinfo->gpio_lowpower_cmds1_len = ARRAY_SIZE(gpio_reset_low_cmds);
	pinfo->gpio_lowpower_cmds1 = gpio_reset_low_cmds;

	/*gpio reset free*/
	static struct gpio_desc gpio_reset_free_cmds[] = {
		/* reset */
		{ 1, 0, 100,"gpio_lcdkit_reset", &gpio_reset, 0 },
	};
	pinfo->gpio_lowpower_cmds2_len = ARRAY_SIZE(gpio_reset_free_cmds);
	pinfo->gpio_lowpower_cmds2 = gpio_reset_free_cmds;

	/*aod high brightness*/
	pinfo->aod_high_brightness_cmds_len = disp_info->aod.aod_high_brightness_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->aod_high_brightness_cmds), &(disp_info->aod.aod_high_brightness_cmds));

	/*aod low brightness*/
	pinfo->aod_low_brightness_cmds_len = disp_info->aod.aod_low_brightness_cmds.cmd_cnt;
	lcd_kit_aod_cmd_init(&(pinfo->aod_low_brightness_cmds), &(disp_info->aod.aod_low_brightness_cmds));

	/*pwm pulse switch*/
	pinfo->hbm_fp_prepare_support = disp_info->pwm.support;
	if (pinfo->hbm_fp_prepare_support) {
		pinfo->hbm_fp_prepare_cmds_len =
			disp_info->pwm.hbm_fp_prepare_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_fp_prepare_cmds),
			&(disp_info->pwm.hbm_fp_prepare_cmds));
		pinfo->hbm_fp_post_cmds_len =
			disp_info->pwm.hbm_fp_post_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_fp_post_cmds),
			&(disp_info->pwm.hbm_fp_post_cmds));
		pinfo->hbm_fp_prepare_cmds_vn1_len =
			disp_info->pwm.hbm_fp_prepare_cmds_vn1.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_fp_prepare_cmds_vn1),
			&(disp_info->pwm.hbm_fp_prepare_cmds_vn1));
		pinfo->hbm_fp_post_cmds_vn1_len =
			disp_info->pwm.hbm_fp_post_cmds_vn1.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_fp_post_cmds_vn1),
			&(disp_info->pwm.hbm_fp_post_cmds_vn1));
	}

	/*vci enable */
	static uint32_t gpio_vci = 0;
	if(NULL != power_hdl->lcd_vci.buf) {
		gpio_vci = power_hdl->lcd_vci.buf[1];
	}
	static struct gpio_desc vci_enable_cmds[] = {
		{3, 1, 0,"gpio_lcdkit_vci", &gpio_vci, 1 },
	};
	pinfo->gpio_vci_enable_cmds = vci_enable_cmds;
	pinfo->gpio_vci_enable_cmds_len = ARRAY_SIZE(vci_enable_cmds);

	/*vci request*/
	static struct gpio_desc vci_request_cmds[] = {
		{0, 1, 0,"gpio_lcdkit_vci", &gpio_vci, 0 },
	};
	pinfo->gpio_vci_request_cmds = vci_request_cmds;
	pinfo->gpio_vci_request_cmds_len = ARRAY_SIZE(vci_request_cmds);

	/*vdd ctrl mode*/
	uint32_t vdd_ctrl_mode = 0;
	if(NULL != power_hdl->lcd_vdd.buf) {
		vdd_ctrl_mode = power_hdl->lcd_vdd.buf[0];
	}
	pinfo->vdd_power_ctrl_mode = vdd_ctrl_mode;
	/* power seq */
	pinfo->power_on_seq.arry_data =
		(struct hisi_array_data *)power_seq->power_on_seq.arry_data;
	pinfo->power_on_seq.cnt = power_seq->power_on_seq.cnt;
	pinfo->power_off_seq.arry_data =
		(struct hisi_array_data *)power_seq->power_off_seq.arry_data;
	pinfo->power_off_seq.cnt = power_seq->power_off_seq.cnt;
	pinfo->panel_on_lp_seq.arry_data =
		(struct hisi_array_data *)power_seq->panel_on_lp_seq.arry_data;
	pinfo->panel_on_lp_seq.cnt = power_seq->panel_on_lp_seq.cnt;
	pinfo->panel_on_hs_seq.arry_data =
		(struct hisi_array_data *)power_seq->panel_on_hs_seq.arry_data;
	pinfo->panel_on_hs_seq.cnt = power_seq->panel_on_hs_seq.cnt;
	pinfo->panel_off_hs_seq.arry_data =
		(struct hisi_array_data *)power_seq->panel_off_hs_seq.arry_data;
	pinfo->panel_off_hs_seq.cnt  = power_seq->panel_off_hs_seq.cnt;
	pinfo->panel_off_lp_seq.arry_data =
		(struct hisi_array_data *)power_seq->panel_off_lp_seq.arry_data;
	pinfo->panel_off_lp_seq.cnt = power_seq->panel_off_lp_seq.cnt;
	/* power config */
	pinfo->lcd_vci.buf   = power_hdl->lcd_vci.buf;
	pinfo->lcd_vci.cnt   = power_hdl->lcd_vci.cnt;
	pinfo->lcd_iovcc.buf = power_hdl->lcd_iovcc.buf;
	pinfo->lcd_iovcc.cnt = power_hdl->lcd_iovcc.cnt;
	pinfo->lcd_vdd.buf   = power_hdl->lcd_vdd.buf;
	pinfo->lcd_vdd.cnt   = power_hdl->lcd_vdd.cnt;
	pinfo->lcd_rst.buf   = power_hdl->lcd_rst.buf;
	pinfo->lcd_rst.cnt   = power_hdl->lcd_rst.cnt;
	pinfo->lcd_aod.buf   = power_hdl->lcd_aod.buf;
	pinfo->lcd_aod.cnt   = power_hdl->lcd_aod.cnt;
}

static void lcd_kit_hbm_init(struct hisi_panel_info *pinfo)
{
	if (pinfo == NULL) {
		LCD_KIT_ERR("pinfo is null\n");
		return;
	}

	pinfo->hbm_support = disp_info->hbm.support;
	pinfo->hbm_special_bit_ctrl = disp_info->hbm.hbm_special_bit_ctrl;
	if (pinfo->hbm_support) {
		pinfo->hbm_prepare_cmds_len =
			disp_info->hbm.hbm_prepare_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_prepare_cmds),
			&(disp_info->hbm.hbm_prepare_cmds));
		pinfo->hbm_write_cmds_len = disp_info->hbm.hbm_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_write_cmds),
			&(disp_info->hbm.hbm_cmds));
		pinfo->hbm_post_cmds_len =
			disp_info->hbm.hbm_post_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_post_cmds),
			&(disp_info->hbm.hbm_post_cmds));
		pinfo->hbm_enter_dim_cmds_len =
			disp_info->hbm.enter_dim_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_enter_dim_cmds),
			&(disp_info->hbm.enter_dim_cmds));
		pinfo->hbm_exit_dim_cmds_len =
			disp_info->hbm.exit_dim_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_exit_dim_cmds),
			&(disp_info->hbm.exit_dim_cmds));
		pinfo->hbm_fp_write_cmds_len =
			disp_info->hbm.fp_enter_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_fp_write_cmds),
			&(disp_info->hbm.fp_enter_cmds));
	}
	pinfo->hbm_elvss_support = disp_info->elvss.support;
	if (pinfo->hbm_elvss_support) {
		pinfo->hbm_elvss_prepare_cmds_len =
			disp_info->elvss.elvss_prepare_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_elvss_prepare_cmds),
			&(disp_info->elvss.elvss_prepare_cmds));
		pinfo->hbm_elvss_write_cmds_len =
			disp_info->elvss.elvss_write_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_elvss_write_cmds),
			&(disp_info->elvss.elvss_write_cmds));
		pinfo->hbm_elvss_post_cmds_len =
			disp_info->elvss.elvss_post_cmds.cmd_cnt;
		lcd_kit_aod_cmd_init(&(pinfo->hbm_elvss_post_cmds),
			&(disp_info->elvss.elvss_post_cmds));
	}
}

static int lcd_kit_pinfo_init(char* compatible, struct hisi_panel_info* pinfo)
{
	int ret = LCD_KIT_OK;
	uint32_t pixel_clk = 0;
	uint32_t dpi0_overlap_size_temp = 0;
	uint32_t dpi1_overlap_size_temp = 0;

	if (!compatible) {
		LCD_KIT_ERR("compatible is null\n");
		return LCD_KIT_FAIL;
	}
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	/*panel info*/
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-xres", &pinfo->xres, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-yres", &pinfo->yres, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-width", &pinfo->width, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-height", &pinfo->height, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-bl-type", &pinfo->bl_set_type, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-bl-min", &pinfo->bl_min, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-bl-max", &pinfo->bl_max, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-bl-boot", &pinfo->bl_boot, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-bl-ic-ctrl-type", &pinfo->bl_ic_ctrl_mode, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,blpwm-div", &pinfo->blpwm_div, 0);
	lcd_kit_parse_get_u8_default(compatible, "lcd-kit,bl-work-delay-frame",
		&pinfo->bl_work_delay_frame, 0);
	lcd_kit_parse_get_u8_default(compatible, "lcd-kit,dpi01-set-change",
		 &pinfo->dpi01_exchange_flag, 0);
	lcd_kit_parse_get_u8_default(compatible, "lcd-kit,pxl-clk-to-pll2",
		 &pinfo->pxl_clk_to_pll2, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-cmd-type", &pinfo->type, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-ifbc-type", &pinfo->ifbc_type, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-pxl-clk", &pixel_clk, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,panel-pxl-clk-div", &pinfo->pxl_clk_rate_div, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,h-back-porch", &pinfo->ldi.h_back_porch, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,h-front-porch", &pinfo->ldi.h_front_porch, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,h-pulse-width", &pinfo->ldi.h_pulse_width, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,v-back-porch", &pinfo->ldi.v_back_porch, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,v-front-porch", &pinfo->ldi.v_front_porch, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,v-pulse-width", &pinfo->ldi.v_pulse_width, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-lane-nums", &pinfo->mipi.lane_nums, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-color-mode", &pinfo->mipi.color_mode, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-vc", &pinfo->mipi.vc, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-dsi-bit-clk", &pinfo->mipi.dsi_bit_clk, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-max-tx-esc-clk", &pinfo->mipi.max_tx_esc_clk, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-burst-mode", &pinfo->mipi.burst_mode, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-non-continue-enable", &pinfo->mipi.non_continue_en, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-clk-post-adjust", &pinfo->mipi.clk_post_adjust, 0);
	lcd_kit_parse_get_u32_default(compatible,
		"lcd-kit,mipi-data-t-lpx-adjust",
		(uint32_t *)(&pinfo->mipi.data_t_lpx_adjust), 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-data-t-hs-trial-adjust", &pinfo->mipi.data_t_hs_trial_adjust, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-rg-vrefsel-vcm-adjust", &pinfo->mipi.rg_vrefsel_vcm_adjust, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-phy-mode", &pinfo->mipi.phy_mode, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,mipi-lp11-flag", &pinfo->mipi.lp11_flag, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,ldi-dpi0-overlap-size", &dpi0_overlap_size_temp, 0);
	lcd_kit_parse_get_u32_default(compatible, "lcd-kit,ldi-dpi1-overlap-size", &dpi1_overlap_size_temp, 0);

	pinfo->ldi.dpi0_overlap_size = (uint8_t)dpi0_overlap_size_temp;
	pinfo->ldi.dpi1_overlap_size = (uint8_t)dpi1_overlap_size_temp;
	pinfo->orientation = 1;
	pinfo->bpp = 0;
	pinfo->bgr_fmt = 0;
	pinfo->ldi.hsync_plr = 0;
	pinfo->ldi.vsync_plr = 0;
	pinfo->ldi.pixelclk_plr = 0;
	pinfo->ldi.data_en_plr = 0;
	pinfo->pxl_clk_rate = pixel_clk * 1000000UL;
	pinfo->lcd_type = LCD_KIT;
	pinfo->lcd_name = disp_info->lcd_name;
	pinfo->mipi.dsi_bit_clk_upt = pinfo->mipi.dsi_bit_clk;
	pinfo->mipi.max_tx_esc_clk = pinfo->mipi.max_tx_esc_clk * 1000000;
	/*aod init*/
	pinfo->aod_support = disp_info->aod.support;
	pinfo->lcd_uninit_step_support = 1;
	if (pinfo->aod_support) {
		lcd_kit_hbm_init(pinfo);
		lcd_kit_aod_init(pinfo);
	}
	return ret;
}

int lcd_kit_parse_dt(char* compatible)
{
	if (!compatible) {
		LCD_KIT_ERR("compatible is null");
		return LCD_KIT_FAIL;
	}
	/*parse display info*/
	lcd_kit_parse_disp_info(compatible);
	return LCD_KIT_OK;
}

int lcd_kit_utils_init(struct hisi_panel_info* pinfo)
{
	int ret = LCD_KIT_OK;

	/*parse panel dts*/
	lcd_kit_parse_dt(disp_info->compatible);
	/*pinfo init*/
	lcd_kit_pinfo_init(disp_info->compatible, pinfo);
	/*parse vesa parameters*/
	lcd_kit_vesa_para_parse(disp_info->compatible, pinfo);
	/*config compress setting*/
	lcd_kit_compress_config(pinfo->ifbc_type, pinfo);
	if (disp_info->dynamic_gamma_support) {
		ret = lcd_kit_write_gm_to_reserved_mem();
		if (ret < LCD_KIT_OK) {
			LCD_KIT_ERR("Writing the gamma data to shared memory is failed!");
		}
	}
	return ret;
}

void lcd_logo_checksum_set(struct hisi_fb_data_type *hisifd)
{
	int ret;

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return;
	}
	if (disp_info->logo_checksum.support) {
		ret = lcd_kit_dsi_cmds_tx(hisifd,
			&disp_info->logo_checksum.enable_cmds);
		if (ret) {
			LCD_KIT_ERR("enable checksum fail\n");
			return;
		}
		LCD_KIT_INFO("enable logo checksum\n");
	}
}

void lcd_logo_checksum_check(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int checksum_result = 0;
	int i;
	uint8_t read_value[LCD_KIT_LOGO_CHECKSUM_SIZE] = {0};

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return;
	}
	if (disp_info->logo_checksum.support) {
		ret = lcd_kit_dsi_cmds_rx(hisifd, read_value,
			&disp_info->logo_checksum.checksum_cmds);
		if (ret) {
			LCD_KIT_ERR("checksum_cmds fail\n");
			return;
		}

		if (disp_info->logo_checksum.value.buf == NULL) {
			LCD_KIT_ERR("logo checksum value is Null!\n");
			return;
		}
		for (i = 0; i < disp_info->logo_checksum.value.cnt; i++) {
			if (read_value[i] != disp_info->logo_checksum.value.buf[i]) {
				LCD_KIT_INFO("read[%d] = 0x%x,buf[%d] = 0x%x\n",
					i, read_value[i], i,
					disp_info->logo_checksum.value.buf[i]);
				checksum_result++;
			}
		}

		if (checksum_result) {
			LCD_KIT_INFO("logo checksum_result:%d\n",
				checksum_result);
			ret = lcd_kit_change_dts_value(
				"huawei,lcd_panel_type",
				"fastboot_record_bit", BIT(1));
			if (ret) {
				LCD_KIT_ERR("change BIT(1) fail\n");
				return;
			}
		}

		ret = lcd_kit_dsi_cmds_tx(hisifd,
			&disp_info->logo_checksum.disable_cmds);
		if (ret) {
			LCD_KIT_ERR("disable checksum fail\n");
			return;
		}
	}
}
