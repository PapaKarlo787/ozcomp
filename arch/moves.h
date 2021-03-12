void mov_rm() {
	readRegisters();
	R[r1] = readNum(readNum());
}

void movb_rm() {
	readRegisters();
	R[r1] = read_(readNum());
}

void mov_mr() {
	readRegisters();
	write_(readNum(), (uint8_t*)&R[r1], 4);
}

void movb_mr() {
	readRegisters();
	write_(readNum(), (uint8_t*)&R[r1], 1);
}

void mov_rr() {
	readRegisters();
	R[r1] = R[r2];
}

void mov_rc() {
	readRegisters();
	R[r1] = readNum();
}

void mov_rmor() {
	readRegisters();
	R[r1] = readNum(readNum() + R[r2]);
}

void movb_rmor() {
	readRegisters();
	R[r1] = read_(readNum() + R[r2]);
}

void mov_morr() {
	readRegisters();
	write_(readNum() + R[r2], (uint8_t*)&R[r1], 4);
}

void movb_morr() {
	readRegisters();
	write_(readNum() + R[r2], (uint8_t*)&R[r1], 1);
}

void imovf() {
	readRegisters();
	R[r1] = (int)Rf[r2];
}
