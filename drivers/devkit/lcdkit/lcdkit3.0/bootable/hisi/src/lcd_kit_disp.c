#include <boardid.h>
#include "hisi_fb.h"
#include "lcd_kit_disp.h"
#include <oeminfo_ops.h>
#include "lcd_kit_effect.h"
#include "lcd_kit_power.h"
#include "lcd_kit_utils.h"

static struct lcd_kit_disp_desc g_lcd_kit_disp_info;
static struct hisi_panel_info lcd_pinfo = {0};
struct lcd_kit_disp_desc *lcd_kit_get_disp_info(void)
{
	return &g_lcd_kit_disp_info;
}

static int lcd_kit_panel_on(struct hisi_fb_panel_data* pdata, struct hisi_fb_data_type* hisifd)
{
	struct hisi_panel_info* pinfo = NULL;
	int ret = LCD_KIT_OK;

	if (!hisifd || !pdata) {
		LCD_KIT_ERR("hisifd or pdata is NULL!\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("fb%d, +!\n", hisifd->index);
	pinfo = hisifd->panel_info;
	if (!pinfo) {
		LCD_KIT_ERR("panel_info is NULL!\n");
		return LCD_KIT_FAIL;
	}

	switch (pinfo->lcd_init_step) {
		case LCD_INIT_POWER_ON:
			ret = common_ops->panel_power_on(hisifd);
			pinfo->lcd_init_step = LCD_INIT_MIPI_LP_SEND_SEQUENCE;
			break;
		case LCD_INIT_MIPI_LP_SEND_SEQUENCE:
			ret = common_ops->panel_on_lp(hisifd);
			lcd_kit_get_tp_color(hisifd);
			lcd_kit_get_elvss_info(hisifd);
			lcd_kit_panel_version_init(hisifd);
			if (disp_info->brightness_color_uniform.support) {
				lcd_kit_bright_rgbw_id_from_oeminfo_process(hisifd);
			}
			pinfo->lcd_init_step = LCD_INIT_MIPI_HS_SEND_SEQUENCE;
			break;
		case LCD_INIT_MIPI_HS_SEND_SEQUENCE:
			ret = common_ops->panel_on_hs(hisifd);
			break;
		case LCD_INIT_NONE:
			break;
		case LCD_INIT_LDI_SEND_SEQUENCE:
			break;
		default:
			break;
	}
	LCD_KIT_INFO("fb%d, -!\n", hisifd->index);
	return ret;
}

static int lcd_kit_panel_off(struct hisi_fb_panel_data* pdata, struct hisi_fb_data_type* hisifd)
{
	struct hisi_panel_info* pinfo = NULL;
	int ret = LCD_KIT_OK;

	if (!hisifd || !pdata) {
		LCD_KIT_ERR("hisifd or pdata is NULL!\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("fb%d, +!\n", hisifd->index);
	pinfo = hisifd->panel_info;
	if (!pinfo) {
		LCD_KIT_ERR("panel_info is NULL!\n");
		return LCD_KIT_FAIL;
	}
	switch (pinfo->lcd_uninit_step) {
		case LCD_UNINIT_MIPI_HS_SEND_SEQUENCE:
			ret = common_ops->panel_off_hs(hisifd);
			pinfo->lcd_uninit_step = LCD_UNINIT_MIPI_LP_SEND_SEQUENCE;
			break;
		case LCD_UNINIT_MIPI_LP_SEND_SEQUENCE:
			ret = common_ops->panel_off_lp(hisifd);
			pinfo->lcd_uninit_step = LCD_UNINIT_POWER_OFF;
			break;
		case LCD_UNINIT_POWER_OFF:
			ret = common_ops->panel_power_off(hisifd);
			break;
		default:
			break;
	}
	LCD_KIT_INFO("fb%d, -!\n", hisifd->index);
	return ret;
}

static int  lcd_kit_set_backlight(struct hisi_fb_panel_data* pdata,
								  struct hisi_fb_data_type* hisifd, uint32_t bl_level)
{
	uint32_t bl_type = 0;
	int ret = LCD_KIT_OK;
	struct hisi_panel_info* pinfo = NULL;

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	/*quickly sleep*/
	if (disp_info->quickly_sleep_out.support) {
		if (disp_info->quickly_sleep_out.interval> 0) {
			mdelay(disp_info->quickly_sleep_out.interval);
		}
	}
	pinfo = hisifd->panel_info;
	/*mapping bl_level from bl_max to 255 step*/
	bl_level = bl_level * pinfo->bl_max/255;
	bl_type = lcd_kit_get_backlight_type(pinfo);
	switch (bl_type) {
		case BL_SET_BY_PWM:
			ret = lcd_kit_pwm_set_backlight(hisifd, bl_level);
			break;
		case BL_SET_BY_BLPWM:
			ret = lcd_kit_blpwm_set_backlight(hisifd, bl_level);
			break;
		case BL_SET_BY_SH_BLPWM:
			ret = lcd_kit_sh_blpwm_set_backlight(hisifd, bl_level);
			break;
		case BL_SET_BY_MIPI:
			ret = lcd_kit_set_mipi_backlight(hisifd, bl_level);
			break;
		default:
			break;
	}
	LCD_KIT_INFO("bl_level = %d, bl_type = %d\n", bl_level, bl_type);
	return ret;
}

/*panel data*/
static struct hisi_fb_panel_data lcd_kit_panel_data = {
	.on             = lcd_kit_panel_on,
	.off            = lcd_kit_panel_off,
	.set_backlight  = lcd_kit_set_backlight,
	.next           = NULL,
};

static int lcd_kit_probe(struct hisi_fb_data_type* hisifd)
{
	struct hisi_panel_info* pinfo = NULL;
	int ret = LCD_KIT_OK;

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is NULL!\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO(" enter\n");
	// init lcd panel info
	pinfo = &lcd_pinfo;
	memset_s(pinfo, sizeof(struct hisi_panel_info), 0, sizeof(struct hisi_panel_info));
	/*common init*/
	if (common_ops->common_init) {
		common_ops->common_init(disp_info->compatible);
	}
	/*utils init*/
	lcd_kit_utils_init(pinfo);
	/*panel init*/
	lcd_kit_panel_init();
	/*power init*/
	lcd_kit_power_init();
	/*panel chain*/
	hisifd->panel_info = pinfo;
	lcd_kit_panel_data.next = hisifd->panel_data;
	hisifd->panel_data = &lcd_kit_panel_data;
	/*add device*/
	hisi_fb_add_device(hisifd);
	LCD_KIT_INFO(" exit\n");
	return ret;
}

struct hisi_fb_data_type lcd_kit_hisifd = {
	.panel_probe = lcd_kit_probe,
};

static void transfer_power_config(u32 *in, struct lcd_kit_array_data *out)
{
	u32 *buf = NULL;
	u8 i;

	if ((in == NULL) || (out == NULL)) {
		LCD_KIT_ERR("param invalid!\n");
		return;
	}
	buf = (u32 *)alloc(LCD_POWER_LEN * sizeof(u32));
	if (!buf) {
		LCD_KIT_ERR("alloc buf fail\n");
		return;
	}
	for (i = 0; i < LCD_POWER_LEN; i++) {
		buf[i] = in[i];
	}
	out->buf = buf;
	out->cnt = LCD_POWER_LEN;
}

static void get_power_config_from_dts(struct lcd_type_operators *ops,
	char *str, struct lcd_kit_array_data *out)
{
	int ret;
	uint32_t power[LCD_POWER_LEN] = {0};

	if ((ops == NULL) || (str == NULL) || (out == NULL)) {
		LCD_KIT_ERR("input param is NULL\n");
		return;
	}
	ret = ops->get_power_by_str(str, power);
	if (ret == LCD_KIT_OK) {
		transfer_power_config(power, out);
		if (out->buf != NULL)
			LCD_KIT_INFO("%s 0x%x, 0x%x, 0x%x\n", str, out->buf[0],
				out->buf[1], out->buf[2]);
	}
}

static void get_power_config_by_str(struct lcd_type_operators *ops)
{
	if (ops == NULL) {
		LCD_KIT_ERR("ops is NULL\n");
		return;
	}
	get_power_config_from_dts(ops, "lcd_vci", &power_hdl->lcd_vci);
	get_power_config_from_dts(ops, "lcd_iovcc", &power_hdl->lcd_iovcc);
	get_power_config_from_dts(ops, "lcd_vdd", &power_hdl->lcd_vdd);
	get_power_config_from_dts(ops, "lcd_vsp", &power_hdl->lcd_vsp);
	get_power_config_from_dts(ops, "lcd_vsn", &power_hdl->lcd_vsn);
	get_power_config_from_dts(ops, "lcd_aod", &power_hdl->lcd_aod);
	get_power_config_from_dts(ops, "lcd_rst", &power_hdl->lcd_rst);
	get_power_config_from_dts(ops, "lcd_te0", &power_hdl->lcd_te0);
}

static int lcd_kit_init(struct system_table* systable)
{
	int lcd_type = UNKNOWN_LCD;
	int pin_num = 0;
	struct lcd_type_operators* lcd_type_ops = NULL;

	lcd_type_ops = get_operators(LCD_TYPE_MODULE_NAME_STR);
	if (!lcd_type_ops) {
		LCD_KIT_ERR("failed to get lcd type operator!\n");
	} else {
		lcd_type = lcd_type_ops->get_lcd_type();
		LCD_KIT_INFO("lcd_type = %d\n", lcd_type);
		if  (lcd_type == LCD_KIT) {
			LCD_KIT_INFO("lcd type is LCD_KIT.\n");
			/*init lcd id*/
			disp_info->lcd_id = lcd_type_ops->get_lcd_id(&pin_num);
			disp_info->product_id = lcd_type_ops->get_product_id();
			disp_info->compatible = lcd_kit_get_compatible(disp_info->product_id, disp_info->lcd_id, pin_num);
			disp_info->lcd_name = lcd_kit_get_lcd_name(disp_info->product_id, disp_info->lcd_id, pin_num);
			lcd_type_ops->set_lcd_panel_type(disp_info->compatible);
			lcd_type_ops->set_hisifd(&lcd_kit_hisifd);
			get_power_config_by_str(lcd_type_ops);
			/*adapt init*/
			lcd_kit_adapt_init();
			LCD_KIT_INFO("disp_info->lcd_id = %d, disp_info->product_id = %d\n",
				disp_info->lcd_id, disp_info->product_id);
			LCD_KIT_DEBUG("disp_info->lcd_name = %s, disp_info->compatible = %s\n",
				disp_info->lcd_name,disp_info->compatible);
		} else {
			LCD_KIT_INFO("lcd type is not LCD_KIT.\n");
		}
	}
	return LCD_KIT_OK;
}

static struct module_data lcd_kit_module_data = {
	.name = LCD_KIT_MODULE_NAME_STR,
	.level   = LCDKIT_MODULE_LEVEL,
	.init = lcd_kit_init,
};

MODULE_INIT(LCD_KIT_MODULE_NAME, lcd_kit_module_data);
