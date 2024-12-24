inline uint8_t read_(){
	uint8_t temp;
	sd_raw_read(ip, &temp, 1);
	ip++;
	return temp;
}

inline uint8_t read_(uint32_t poi){
	uint8_t temp;
	sd_raw_read(poi, &temp, 1);
	return temp;
}

inline void readRegisters() {
	uint8_t i = read_();
	r1 = i & 15;
	r2 = i >> 4;
}

inline uint32_t readNum() {
	uint32_t temp;
	sd_raw_read(ip, (uint8_t*)&temp, 4);
	ip += 4;
	return temp;
}

inline uint32_t readNum(uint32_t poi, uint8_t n = 4) {
	uint32_t temp = 0;
	sd_raw_read(poi, (uint8_t*)&temp, n);
	return temp;
}

inline void setFlags(float x) {
	flags &= 0xffff0000;
	flags += (x > 0 ? 1 : 0) << 6;
	flags += (x < 0 ? 1 : 0) << 5;
	flags += (x == 0 ? 1 : 0) << 4;
}

inline void setFlags(uint32_t x) {
	int32_t y = x;
	flags &= 0xffff0000;
	flags += (y > 0 ? 1 : 0) << 2;
	flags += (y < 0 ? 1 : 0) << 1;
	flags += y == 0 ? 1 : 0;
}

inline void setFlags(bool x) {
	flags &= 0xffff0000;
	flags |= x ? 8 : 128;
}

inline void write_(void* data, uint8_t n){
	sd_raw_write(ip, (uint8_t*)data, n);
}

inline void write_(uint32_t poi, void* data, uint8_t n){
	sd_raw_write(poi, (uint8_t*)data, n);
}

void begin() {
	randomSeed(analogRead(0));
	digitalWrite(A4, 1);
	while (!sd_raw_init()) {};
	digitalWrite(A4, 0);
	digitalWrite(A1, 1);
	kbd.begin();
	digitalWrite(A1, 0);

	lcd.begin();
	timeUnix.begin();
	flags = 0x30000;
	// High flags
	// 0)	color
	// 1)	sound rgb
}
