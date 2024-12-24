void fmov_rm() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum(lea(r2, 0), 4);
	Rf[r1] = v.f;
}

void fmov_mr() {
	readRegisters();
	write_(lea(r2, 0), &Rf[r1], 4);
}

void fmov_rr() {
	readRegisters();
	Rf[r1] = Rf[r2];
}

void fmov_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	Rf[r1] = v.f;
}

void fmovi() {
	readRegisters();
	Rf[r1] = R[r2];
}
