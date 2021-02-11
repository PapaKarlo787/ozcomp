void and_rr() {
	readRegisters();
	setFlags(R[r1] = (int)R[r1] & (int)R[r2]);
}

void and_rc() {
	readRegisters();
	unsigned int x = readNum(4);
	setFlags(R[r1] = (int)R[r1] & (int)(*(float*)&x));
}

void or_rr() {
	readRegisters();
	setFlags(R[r1] = (int)R[r1] | (int)R[r2]);
}

void or_rc() {
	readRegisters();
	unsigned int x = readNum(4);
	setFlags(R[r1] = (int)R[r1] | (int)(*(float*)&x));
}

void xor_rr() {
	readRegisters();
	setFlags(R[r1] = (int)R[r1] ^ (int)R[r2]);
}

void xor_rc() {
	readRegisters();
	unsigned int x = readNum(4);
	setFlags(R[r1] = (int)R[r1] ^ (int)(*(float*)&x));
}
