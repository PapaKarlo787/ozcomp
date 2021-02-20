void mov_rm() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4);
	R[r1] = readNum(4);
	ip = temp + 4;
}

void movb_rm() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4);
	R[r1] = readNum(1);
	ip = temp + 4;
}

void mov_mr() {
	readRegisters();
	write_(readNum(4), (uint8_t*)&R[r1], 4);
}

void movb_mr() {
	readRegisters();
	write_(readNum(4), (uint8_t*)&R[r1], 1);
}

void mov_rr() {
	readRegisters();
	R[r1] = R[r2];
}

void mov_rc() {
	readRegisters();
	R[r1] = readNum(4);
}

void mov_rmor() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4) + R[r2];
	R[r1] = readNum(4);
	ip = temp + 4;
}

void movb_rmor() {
	readRegisters();
	uint32_t temp = ip;
	ip = readNum(4) + R[r2];
	R[r1] = readNum(1);
	ip = temp + 4;
}

void mov_morr() {
	readRegisters();
	write_(readNum(4) + R[r2], (uint8_t*)&R[r1], 4);
}

void movb_morr() {
	readRegisters();
	write_(readNum(4)+R[r2], (uint8_t*)&R[r1], 1);
}
