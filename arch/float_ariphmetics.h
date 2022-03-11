#include <cmath>

void f_add_rr() {
	readRegisters();
	setFlags(Rf[r1] += Rf[r2]);
}

void f_add_rc() {
	readRegisters();
	uint32_t x = readNum();
	setFlags(Rf[r1] = Rf[r1] + *(float*)&x);
}

void f_sub_rr() {
	readRegisters();
	setFlags(Rf[r1] -= Rf[r2]);
}

void f_sub_rc() {
	readRegisters();
	uint32_t x = readNum();
	setFlags(Rf[r1] -= *(float*)&x);
}
	
void f_mul_rr() {
	readRegisters();
	setFlags(Rf[r1] *= Rf[r2]);
}

void f_mul_rc() {
	readRegisters();
	uint32_t x = readNum();
	setFlags(Rf[r1] *= *(float*)&x);
}

void f_div_rr() {
	readRegisters();
	setFlags(Rf[r1] /= Rf[r2]);
}

void f_div_rc() {
	readRegisters();
	uint32_t x = readNum();
	setFlags(Rf[r1] /= *(float*)&x);
}

void f_pow_rr() {
	readRegisters();
	Rf[r1] = pow(Rf[r1], Rf[r2]);
}

void f_pow_rc() {
	readRegisters();
	uint32_t x = readNum();
	Rf[r1] = pow(Rf[r1], *(float*)&x);
}

void f_cmp_rr() {
	readRegisters();
	setFlags(Rf[r1] - Rf[r2]);
}

void f_cmp_rc() {
	readRegisters();
	uint32_t x = readNum();
	setFlags(Rf[r1] - *(float*)&x);
}

void test_f() {
	readRegisters();
	setFlags(Rf[r1]);
}
