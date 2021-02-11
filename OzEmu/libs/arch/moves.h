void mov_rm() {
	readRegisters();
	unsigned int temp = ip;
	ip = readNum(4);
	unsigned int x = readNum(4);
	R[r1] = *(float*)&x;
	ip = temp + 4;
}

void movb_rm() {
	readRegisters();
	unsigned int temp = ip;
	ip = readNum(4);
	R[r1] = readNum(1);
	ip = temp + 4;
}

void mov_mr() {
	readRegisters();
	write(readNum(4), (unsigned char*)&(R[r1]), 4);
}

void movb_mr() {
	readRegisters();
	char x = (char)R[r1];
	write(readNum(4), (unsigned char*)&(x), 1);
}

void mov_rr() {
	readRegisters();
	R[r1] = R[r2];
}

void mov_rc() {
	readRegisters();
	unsigned int x = readNum(4);
	R[r1] = *(float*)&x;
}

void mov_rmor() {
	readRegisters();
	unsigned int temp = ip;
	ip = readNum(4)+((int)R[r2]);
	unsigned int x = readNum(4);
	R[r1] = *(float*)&x;
	ip = temp + 4;
}

void movb_rmor() {
	readRegisters();
	unsigned int temp = ip;
	ip = readNum(4)+((int)R[r2]);
	R[r1] = readNum(1);
	ip = temp + 4;
}

void mov_morr() {
	readRegisters();
	write(readNum(4)+((int)R[r2]), (unsigned char*)&(R[r1]), 4);
}

void movb_morr() {
	readRegisters();
	char x = (char)R[r1];
	write(readNum(4)+((int)R[r2]), (unsigned char*)&(x), 1);
}
