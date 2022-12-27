void fmov_rm() {
	readRegisters();
	uint32_t x = readNum(mov(r2, 0), 4);
	Rf[r1] = *(float*)&x;
}

void fmov_mr() {
	readRegisters();
	write_(mov(r2, 0), (uint8_t*)&Rf[r1], 4);
}

void fmov_rr() {
	readRegisters();
	Rf[r1] = Rf[r2];
}

void fmov_rc() {
	readRegisters();
	uint32_t x = readNum();
	Rf[r1] = *(float*)&x;
}

void fmovi() {
	readRegisters();
	Rf[r1] = R[r2];
}
