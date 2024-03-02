

#if defined(GC5025_HOLI)
{
	.sensorID = GC5025_HOLI_SENSOR_ID,
	.e2prom_block_num = 2,
	.e2prom_total_size = 1940,
	.e2prom_block_info[0] = {
		.AddrOffset = 0x00,
		.datalen = 39,
	},
	.e2prom_block_info[1] = {
		.AddrOffset = 0x27,
		.datalen = 1901,
	},
},
#endif
