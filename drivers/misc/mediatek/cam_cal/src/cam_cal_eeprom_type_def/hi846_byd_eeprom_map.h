

#if defined(HI846_BYD)
{
	.sensorID = HI846_BYD_SENSOR_ID,
	.e2prom_block_info[0] = {
		.AddrOffset = 0x00,
		.datalen = 39,
	},
	.e2prom_block_info[1] = {
		.AddrOffset = 0x15ea,
		.datalen = 2400,
	},
	.e2prom_block_num = 2,
	.e2prom_total_size = 2439,
},
#endif
