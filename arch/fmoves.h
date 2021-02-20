void fmov_rm() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4);
	uint32_t x = readNum(4);
	Rf[r1] = *(float*)&x;
	ip = temp + 4;
}

void fmov_mr() {
	readRegisters();
	write_(readNum(4), (uint8_t*)&Rf[r1], 4);
}

void fmov_rr() {
	readRegisters();
	Rf[r1] = Rf[r2];
}

void fmov_rc() {
	readRegisters();
	uint32_t x = readNum(4);
	Rf[r1] = *(float*)&x;
}

void fmov_rmor() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4) + R[r2];
	uint32_t x = readNum(4);
	Rf[r1] = *(float*)&x;
	ip = temp + 4;
}

void fmov_morr() {
	readRegisters();
	write_(readNum(4) + R[r2], (uint8_t*)&Rf[r1], 4);
}
