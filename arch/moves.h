// most common address [const+Ra+Rb*x] where x in {1, 2, 4}
// mask = xxyy
// xx - size of tranfering data 
//		(11 means that Ra not includes in address and index "a" contains
//							size of transfering data, otherwise 1 << xx)
// yy - scale of Rb
//		(11 means that x = 0, otherwise 1 << yy)

uint32_t mov(uint8_t mask, uint8_t* s) {
	readRegisters();
	uint8_t x = (mask & 3 == 3) ? 0 : (1 << (mask & 3));
	mask >>= 2;
	if (s)
		*s = 1 << ((mask == 3) ? r1 : mask);
	return readNum() + (mask == 3 ? 0 : R[r1]) + R[r2]*x;
}

void mov_rm() {
	readRegisters();
	uint8_t x = r1;
	uint8_t s;
	uint32_t addr = mov(r2, &s);
	R[x] = readNum(addr, s);
}

void mov_mr() {
	readRegisters();
	uint8_t x = r1;
	uint8_t s;
	uint32_t addr = mov(r2, &s);
	write_(addr, (uint8_t*)&R[x], s);
}

void mov_rr() {
	readRegisters();
	R[r1] = R[r2];
}

void mov_rc() {
	readRegisters();
	R[r1] = readNum();
}

void imovf() {
	readRegisters();
	R[r1] = (int)Rf[r2];
}

void lea() {
	readRegisters();
	R[r1] = mov(r2, 0);
}
