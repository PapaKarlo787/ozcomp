void add_rr() {
	readRegisters();
	setFlags(R[r1] += R[r2]);
}

void add_rc() {
	readRegisters();
	setFlags(R[r1] += readNum(4));
}

void sub_rr() {
	readRegisters();
	setFlags(R[r1] -= R[r2]);
}

void sub_rc() {
	readRegisters();
	setFlags(R[r1] -= readNum(4));
}
	
void mul_rr() {
	readRegisters();
	setFlags(R[r1] *= R[r2]);
}

void mul_rc() {
	readRegisters();
	setFlags(R[r1] *= readNum(4));
}

void div_rr() {
	readRegisters();
	setFlags(R[r1] /= R[r2]);
}

void div_rc() {
	readRegisters();
	setFlags(R[r1] /= readNum(4));
}

void pow_rr() {
	readRegisters();
	R[r1] = pow(R[r1], R[r2]);
}

void pow_rc() {
	readRegisters();
	R[r1] = pow(R[r1], readNum(4));
}

void cmp_rr() {
	readRegisters();
	setFlags(R[r1] - R[r2]);
}

void cmp_rc() {
	readRegisters();
	setFlags(R[r1] - readNum(4));
}

void mod_rr() {
	readRegisters();
	setFlags(R[r1] %= R[r2]);
}

void mod_rc() {
	readRegisters();
	setFlags(R[r1] %= readNum(4));
}
