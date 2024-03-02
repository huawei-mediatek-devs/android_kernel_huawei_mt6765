

#ifndef __CAM_CAL_LIST_H
#define __CAM_CAL_LIST_H
#include <linux/i2c.h>

typedef unsigned int (*cam_cal_cmd_func)(struct i2c_client *client,
		unsigned int addr, unsigned char *data, unsigned int size);

struct stCAM_CAL_LIST_STRUCT {
	unsigned int sensorID;
	unsigned int slaveID;
	cam_cal_cmd_func readCamCalData;
};

unsigned int cam_cal_get_sensor_list(struct stCAM_CAL_LIST_STRUCT **ppCamcalList);

/* Common EEPROM read i2C function */
unsigned int Common_read_region(struct i2c_client *client, unsigned int addr,
				unsigned char *data, unsigned int size);

#endif /* __CAM_CAL_LIST_H */
