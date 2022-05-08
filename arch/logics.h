void and_rr() {
	readRegisters();
	setFlags(R[r1] = R[r1] & R[r2]);
}

void and_rc() {
	readRegisters();
	setFlags(R[r1] &= readNum());
}

void or_rr() {
	readRegisters();
	setFlags(R[r1] |= R[r2]);
}

void or_rc() {
	readRegisters();
	setFlags(R[r1] |= readNum());
}

void xor_rr() {
	readRegisters();
	setFlags(R[r1] ^= R[r2]);
}

void xor_rc() {
	readRegisters();
	setFlags(R[r1] ^= readNum());
}

void shr_rr() {
	readRegisters();
	setFlags(R[r1] >>= R[r2]);
}

void shr_rc() {
	readRegisters();
	setFlags(R[r1] >>= read_());
}

void shl_rr() {
	readRegisters();
	setFlags(R[r1] <<= R[r2]);
}

void shl_rc() {
	readRegisters();
	setFlags(R[r1] <<= read_());
}

void test_rr() {
	readRegisters();
	setFlags(R[r1] & R[r2]);
}

void test_rc() {
	readRegisters();
	setFlags(R[r1] & readNum());
}
