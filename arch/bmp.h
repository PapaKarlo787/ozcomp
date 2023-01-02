void bmp(uint8_t x, uint8_t y) {
	uint32_t t = ip;
	ip = readNum();
	setFlags(lcd.draw_bmp(x, y, read_, &ip));
	ip = t+4;
}

void bmp_rc() {
	readRegisters();
	bmp(R[r1], read_());
}

void bmp_cr() {
	uint8_t x = read_();
	readRegisters();
	bmp(x, R[r1]);
}

void bmp_rr() {
	readRegisters();
	bmp(R[r1], R[r2]);
}

void bmp_cc() {
	bmp(read_(), read_());
}
