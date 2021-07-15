uint8_t read_(){
	uint8_t temp[1];
	sd_raw_read(ip, temp, 1);
	ip++;
	return temp[0];
}

uint8_t read_(uint32_t poi){
	uint8_t temp[1];
	sd_raw_read(poi, temp, 1);
	return temp[0];
}

void readRegisters() {
	uint8_t i = read_();
	r1 = i & 15;
	r2 = i >> 4;
}

uint32_t readNum() {
	uint8_t temp[4];
	sd_raw_read(ip, temp, 4);
	ip+=4;
	return *((uint32_t*)&temp);
}

uint32_t readNum(uint32_t poi) {
	uint8_t temp[4];
	sd_raw_read(poi, temp, 4);
	return *((uint32_t*)&temp);
}

void setFlags(float x) {
	flags &= 143;
	flags += (x > 0 ? 1 : 0) << 6;
	flags += (x < 0 ? 1 : 0) << 5;
	flags += (x == 0 ? 1 : 0) << 4;
}

void setFlags(uint32_t x) {
	int32_t y = x;
	flags &= 248;
	flags += (y > 0 ? 1 : 0) << 2;
	flags += (y < 0 ? 1 : 0) << 1;
	flags += y == 0 ? 1 : 0;
}

void write_(uint8_t* data, uint8_t n){
	sd_raw_write(ip, data, n);
	sd_raw_sync();
}

void write_(uint32_t poi, uint8_t* data, uint8_t n){
	sd_raw_write(poi, data, n);
	sd_raw_sync();
}
