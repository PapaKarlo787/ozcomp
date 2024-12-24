#ifdef OZEMU
  #include <cmath>
#endif

void f_add_rr() {
	readRegisters();
	setFlags(Rf[r1] += Rf[r2]);
}

void f_add_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	setFlags(Rf[r1] += v.f);
}

void f_sub_rr() {
	readRegisters();
	setFlags(Rf[r1] -= Rf[r2]);
}

void f_sub_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	setFlags(Rf[r1] -= v.f);
}
	
void f_mul_rr() {
	readRegisters();
	setFlags(Rf[r1] *= Rf[r2]);
}

void f_mul_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	setFlags(Rf[r1] *= v.f);
}

void f_div_rr() {
	readRegisters();
	setFlags(Rf[r1] /= Rf[r2]);
}

void f_div_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	setFlags(Rf[r1] /= v.f);
}

void f_pow_rr() {
	readRegisters();
	Rf[r1] = pow(Rf[r1], Rf[r2]);
}

void f_pow_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	Rf[r1] = pow(Rf[r1], v.f);
}

void f_cmp_rr() {
	readRegisters();
	setFlags(Rf[r1] - Rf[r2]);
}

void f_cmp_rc() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum();
	setFlags(Rf[r1] - v.f);
}

void test_f() {
	readRegisters();
	setFlags(Rf[r1]);
}
