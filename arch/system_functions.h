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

uint32_t readNum(uint32_t poi, uint8_t n = 4) {
	uint8_t temp[4] = {0, 0, 0, 0};
	sd_raw_read(poi, temp, n);
	return *((uint32_t*)&temp);
}

void setFlags(float x) {
	flags &= 0xffff0000;
	flags += (x > 0 ? 1 : 0) << 6;
	flags += (x < 0 ? 1 : 0) << 5;
	flags += (x == 0 ? 1 : 0) << 4;
}

void setFlags(uint32_t x) {
	int32_t y = x;
	flags &= 0xffff0000;
	flags += (y > 0 ? 1 : 0) << 2;
	flags += (y < 0 ? 1 : 0) << 1;
	flags += y == 0 ? 1 : 0;
}

void setFlags(bool x) {
	flags &= 0xffff0000;
	flags |= x ? 8 : 128;
}

void write_(uint8_t* data, uint8_t n){
	sd_raw_write(ip, data, n);
}

void write_(uint32_t poi, uint8_t* data, uint8_t n){
	sd_raw_write(poi, data, n);
}

void begin() {
	analogWrite(A1, 1000);
	while (!sd_raw_init()) {};
	analogWrite(A1, 500);
	lcd.begin();
	kbd.write(0xF0); //Установить Scan Code
	kbd.read();
	kbd.write(0x01); //...3
	kbd.read();
	kbd.write(0xF8); // Режим make/release
	kbd.read();
	kbd.begin();
	timeUnix.begin();
	flags = 0x30000;
	// High flags
	// 0)	color
	// 1)	sound rgb
	randomSeed(analogRead(0));
	analogWrite(A1, 0);
#ifdef V1
	analogWrite(15, 0);
#endif
}
