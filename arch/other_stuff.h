void to_int() {
	readRegisters();
	setFlags(Rf[r1] = (uint32_t)Rf[r1]);
}

void del_r() {
	readRegisters();
	delay(R[r1]);
}

void del_c() {
	delay(readNum());
}

void rnd() {
	R[13] = random(1024);
}

void nop() { }

void mzer_r() {
	readRegisters();
	for (uint32_t i = readNum(); i > 0; i--)
		write_(R[r1]+i-1, new uint8_t {0}, 1);	
}

void mzer_c() {
	uint32_t poi = readNum();
	for (uint32_t i = readNum(); i > 0; i--)
		write_(poi+i-1, new uint8_t {0}, 1);
}
