uint8_t read_(){
	uint8_t temp[1];
	sd_raw_read(ip, temp, 1);
	ip++;
	return temp[0];
}

void readRegisters() {
	uint8_t i = read_();
	r1 = i & 15;
	r2 = (i & 240) / 16;
}

uint32_t readNum(uint8_t n) {
	uint32_t x = 0;
	for (uint8_t i = 0; i < n; i++)
		x += ((uint32_t)read_()) << (8 * i);
	return x;
}

void setFlags(float x) {
	flags &= 248;
	flags += x > 0 ? 1 : 0;
	flags *= 2;
	flags += x < 0 ? 1 : 0;
	flags *= 2;
	flags += x == 0 ? 1 : 0;
}

void write_(uint8_t* data, uint8_t n){
	sd_raw_write(ip, data, n);
	sd_raw_sync();
}

void write_(uint32_t poi, uint8_t* data, uint8_t n){
	sd_raw_write(poi, data, n);
	sd_raw_sync();
}
