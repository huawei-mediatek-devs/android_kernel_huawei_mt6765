#$(info cam_cal: makefile start)
MTK_CUSTOM_PATH  := $(srctree)/drivers/misc/mediatek

subdir-ccflags-y += -I$(MTK_CUSTOM_PATH)/imgsensor/inc \
                    -I$(MTK_CUSTOM_PATH)/imgsensor/src/$(CONFIG_MTK_PLATFORM)/camera_hw \
                    -I$(MTK_CUSTOM_PATH)/imgsensor/src/common/v1 \
                    -I$(MTK_CUSTOM_PATH)/cam_cal/inc
subdir-ccflags-y += -I$(srctree)/drivers/i2c/busses/

CUSTOM_KERNEL_FLAGS := $(addprefix -D, $(shell echo \
             $(CONFIG_CUSTOM_KERNEL_IMGSENSOR) | tr a-z A-Z))
subdir-ccflags-y += $(CUSTOM_KERNEL_FLAGS)

LEGACY_PLATFORM = mt6572 mt6580 mt6582 mt6735 mt6752 mt6795 mt6755
CURRENT_PLATFORM = $(subst ",,$(CONFIG_MTK_PLATFORM))

ifneq (, $(findstring $(CURRENT_PLATFORM), $(LEGACY_PLATFORM)))
#$(info cam_cal: use legacy folder)
obj-y += legacy/$(subst ",,$(CONFIG_MTK_PLATFORM))/
else
# $(info cam_cal: use common folder)
obj-y += eeprom_driver.o
obj-y += eeprom_driver_hw_adapt.o
obj-y += eeprom_I2C_driver.o
obj-y += cam_cal_list.o
endif
#$(info cam_cal: makefile end)

