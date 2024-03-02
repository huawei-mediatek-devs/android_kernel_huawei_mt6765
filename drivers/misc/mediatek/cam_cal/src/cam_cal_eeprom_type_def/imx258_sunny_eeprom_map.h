

#if defined(IMX258_SUNNY)
{
	.sensorID = IMX258_SENSOR_ID,
	.e2prom_block_num = 2,
	.e2prom_total_size = 4359,
	.e2prom_block_info[0] = {
		.AddrOffset = 0x00,
		.datalen = 39,
	},
	.e2prom_block_info[1] = {
		.AddrOffset = 0x23c3,
		.datalen = 4320,
	},
},
#endif
