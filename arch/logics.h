void and_rr() {
	readRegisters();
	setFlags(R[r1] = R[r1] & R[r2]);
}

void and_rc() {
	readRegisters();
	setFlags(R[r1] &= readNum(4));
}

void or_rr() {
	readRegisters();
	setFlags(R[r1] |= R[r2]);
}

void or_rc() {
	readRegisters();
	setFlags(R[r1] |= readNum(4));
}

void xor_rr() {
	readRegisters();
	setFlags(R[r1] ^= R[r2]);
}

void xor_rc() {
	readRegisters();
	setFlags(R[r1] ^= readNum(4));
}

void shr_rr() {
	readRegisters();
	setFlags(R[r1] <<= R[r2]);
}

void shr_rc() {
	readRegisters();
	setFlags(R[r1] <<= readNum(4));
}

void shl_rr() {
	readRegisters();
	setFlags(R[r1] >>= R[r2]);
}

void shl_rc() {
	readRegisters();
	setFlags(R[r1] >>= readNum(4));
}
