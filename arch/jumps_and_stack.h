void jmp() {
	ip = readNum();
}

void jmp_r(){
	readRegisters();
	ip = R[r1];
}

void jmp_c_r(){
	readRegisters();
	ip = (read_() & flags) != 0 ? R[r1] : ip + 4;
}

void jmp_c() {
	uint32_t x = read_();
	ip = (x & flags) ? readNum() : ip+4;
	flags &= ~((uint32_t)x);
}

void call_c() {
	uint32_t old_ip = ip + 4;
	write_(sp-=4, (uint8_t*)&old_ip, 4);
	ip = readNum();
}

void call_r() {
	uint32_t old_ip = ip + 4;
	write_(sp-=4, (uint8_t*)&old_ip, 4);
	readRegisters();
	ip = R[r1];
}

void ret() {
	ip = sp;
	ip = readNum();
	sp += 4;
}

void lp() {
	ip = --R[14] > 0 ? readNum() : ip + 4;
}

void lp_r() {
	readRegisters();
	ip = --R[14] > 0 ? R[r1] : ip + 4;
}

void push() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(R[r1]), 4);
}

void push_c() {
	uint32_t x = readNum();
	write_(sp -= 4, (uint8_t*)&x, 4);
}

void pop() {
	readRegisters();
	R[r1] = readNum(sp, 4);
	sp += 4;
}

void pushai() {
	for(uint8_t i = 0; i < 16; i++)
		write_(sp -= 4, (uint8_t*)&(R[i]), 4);
}

void popai() {
	for(int8_t i = 15; i >= 0; i--) {
		R[i] = readNum(sp, 4);
		sp += 4;
	}
}

void fpush() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(Rf[r1]), 4);
}

void fpop() {
	readRegisters();
	uint32_t x = readNum(sp, 4);
	Rf[r1] = *(float*)&x;
	sp += 4;
}

void pushaf() {
	for(uint8_t i = 0; i < 16; i++)
		write_(sp -= 4, (uint8_t*)&(Rf[i]), 4);
}

void popaf() {
	for(int8_t i = 15; i >= 0; i--) {
		uint32_t x = readNum(sp, 4);
		Rf[i] = *(float*)&x;
		sp += 4;
	}
}
