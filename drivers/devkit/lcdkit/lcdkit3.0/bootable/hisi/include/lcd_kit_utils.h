#ifndef __LCD_KIT_UTILS_H_
#define __LCD_KIT_UTILS_H_
#include "lcd_kit_common.h"
#include "hisi_mipi_dsi.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_panel.h"

#define DTS_LCD_PANEL_TYPE  "/huawei,lcd_panel"
#define LCD_KIT_DEFAULT_PANEL   "/huawei,lcd_config/lcd_kit_default_auo_otm1901a_5p2_1080p_video_default"
#define LCD_KIT_DEFAULT_COMPATIBLE   "auo_otm1901a_5p2_1080p_video_default"
#define LCD_DDIC_INFO_LEN 64
#define LCD_1_PIN 1
#define LCD_2_PIN 2
#define DEF_LCD_1_PIN 0x02
#define DEF_LCD_2_PIN 0x0a

#define REG61H_VALUE_FOR_RGBW 3800 // get the same brightness as in fastboot when enter kernel at the first time

#define LCD_KIT_ELVSS_DIM_DEFAULT     0x13
#define LCD_KIT_ELVSS_DIM_ENABLE_MASK 0x80
#define VERSION_VALUE_NUM_MAX 10
#define VERSION_NUM_MAX 20

#define LCD_KIT_LOGO_CHECKSUM_SIZE   8

struct lcd_kit_quickly_sleep_out_desc {
	uint32_t support;
	uint32_t interval;
};

struct lcd_kit_tp_color_desc {
	uint32_t support;
	struct lcd_kit_dsi_panel_cmds cmds;
};

struct lcd_kit_snd_disp {
	u32 support;
	struct lcd_kit_dsi_panel_cmds on_cmds;
	struct lcd_kit_dsi_panel_cmds off_cmds;
};

struct lcd_kit_rgbw {
	u32 support;
	struct lcd_kit_dsi_panel_cmds backlight_cmds;
};

struct lcd_kit_aod {
	u32 support;
	/*enter aod command*/
	struct lcd_kit_dsi_panel_cmds aod_on_cmds;
	/*exit aod command*/
	struct lcd_kit_dsi_panel_cmds aod_off_cmds;
	/* aod high brightness command*/
	struct lcd_kit_dsi_panel_cmds aod_high_brightness_cmds;
	/* aod low brightness command*/
	struct lcd_kit_dsi_panel_cmds aod_low_brightness_cmds;
};

struct lcd_kit_hbm {
	u32 support;
	u32 hbm_special_bit_ctrl;
	struct lcd_kit_dsi_panel_cmds enter_cmds;
	struct lcd_kit_dsi_panel_cmds fp_enter_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_prepare_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_post_cmds;
	struct lcd_kit_dsi_panel_cmds exit_cmds;
	struct lcd_kit_dsi_panel_cmds enter_dim_cmds;
	struct lcd_kit_dsi_panel_cmds exit_dim_cmds;
};

struct lcd_kit_hbm_elvss {
	u32 support;
	u32 set_elvss_lp_support;
	/*elvss dim read cmds*/
	struct lcd_kit_dsi_panel_cmds elvss_prepare_cmds;
	struct lcd_kit_dsi_panel_cmds elvss_write_cmds;
	struct lcd_kit_dsi_panel_cmds elvss_read_cmds;
	struct lcd_kit_dsi_panel_cmds elvss_post_cmds;
};

struct lcd_kit_pwm_pulse_switch {
	u8 support;
	struct lcd_kit_dsi_panel_cmds hbm_fp_prepare_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_fp_post_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_fp_prepare_cmds_vn1;
	struct lcd_kit_dsi_panel_cmds hbm_fp_post_cmds_vn1;
};

struct lcd_kit_panel_version {
	u32 support;
	u32 value_number;
	u32 version_number;
	char read_value[VERSION_VALUE_NUM_MAX];
	char lcd_version_name[VERSION_NUM_MAX][LCD_PANEL_VERSION_SIZE];
	struct lcd_kit_arrays_data value;
	struct lcd_kit_dsi_panel_cmds cmds;
	struct lcd_kit_dsi_panel_cmds enter_cmds;
};

struct lcd_kit_logo_checksum {
	u32 support;
	struct lcd_kit_dsi_panel_cmds checksum_cmds;
	struct lcd_kit_dsi_panel_cmds enable_cmds;
	struct lcd_kit_dsi_panel_cmds disable_cmds;
	struct lcd_kit_array_data value;
};

/*function declare*/
int lcd_kit_get_tp_color(struct hisi_fb_data_type* hisifd);
int lcd_kit_get_elvss_info(struct hisi_fb_data_type* hisifd);
int lcd_kit_panel_version_init(struct hisi_fb_data_type *hisifd);
int lcd_panel_version_compare(struct hisi_fb_data_type *hisifd);
int lcd_kit_adapt_init(void);
uint32_t lcd_kit_get_backlight_type(struct hisi_panel_info* pinfo);
int lcd_kit_utils_init(struct hisi_panel_info* pinfo);
int lcd_kit_dsi_fifo_is_full(uint32_t dsi_base);
void lcd_kit_read_power_status(struct hisi_fb_data_type* hisifd);
int lcd_kit_pwm_set_backlight(struct hisi_fb_data_type *hisifd, uint32_t bl_level);
uint32_t lcd_kit_blpwm_set_backlight(struct hisi_fb_data_type* hisifd, uint32_t bl_level);
int lcd_kit_sh_blpwm_set_backlight(struct hisi_fb_data_type *hisifd, uint32_t bl_level);
int lcd_kit_set_mipi_backlight(struct hisi_fb_data_type* hisifd, uint32_t bl_level);
char* lcd_kit_get_compatible(uint32_t product_id, uint32_t lcd_id, int pin_num);
char* lcd_kit_get_lcd_name(uint32_t product_id, uint32_t lcd_id, int pin_num);
int lcd_kit_dsi_cmds_tx(void* hld, struct lcd_kit_dsi_panel_cmds* cmds);
int lcd_kit_dsi_cmds_rx(void* hld, uint8_t* out, struct lcd_kit_dsi_panel_cmds* cmds);
void lcd_kit_set_mipi_tx_link(struct hisi_fb_data_type *hisifd, int link_state);
void lcd_kit_set_mipi_rx_link(struct hisi_fb_data_type *hisifd, int link_state);
int lcd_kit_change_dts_value(char *compatible, char *dts_name, u32 value);
void lcd_logo_checksum_set(struct hisi_fb_data_type *hisifd);
void lcd_logo_checksum_check(struct hisi_fb_data_type *hisifd);
#endif
