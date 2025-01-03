void jmp() {
	ip = readNum();
}

void jmp_r(){
	readRegisters();
	ip = R[r1];
}

void jmp_c_r(){
	readRegisters();
	ip = (read_() & flags) ? R[r1] : ip + 4;
}

void jmp_c() {
	uint32_t x = read_();
	ip = (x & flags) ? readNum() : ip + 4;
	flags &= ~x;
}

void call_c() {
	uint32_t old_ip = ip + 4;
	write_(sp-=4, &old_ip, 4);
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
	write_(sp -= 4, &R[r1], 4);
}

void push_c() {
	uint32_t x = readNum();
	write_(sp -= 4, &x, 4);
}

void pop() {
	readRegisters();
	R[r1] = readNum(sp, 4);
	sp += 4;
}

void pushai() {
	for(uint8_t i = 0; i < 16; i++)
		write_(sp -= 4, &R[i], 4);
}

void popai() {
	for(int8_t i = 15; i >= 0; i--) {
		R[i] = readNum(sp, 4);
		sp += 4;
	}
}

void fpush() {
	readRegisters();
	write_(sp -= 4, &Rf[r1], 4);
}

void fpop() {
	readRegisters();
	INT_FLOAT v;
	v.i = readNum(sp, 4);
	Rf[r1] = v.f;
	sp += 4;
}

void pushaf() {
	for(uint8_t i = 0; i < 16; i++)
		write_(sp -= 4, &Rf[i], 4);
}

void popaf() {
	INT_FLOAT v;
	for(int8_t i = 15; i >= 0; i--) {
		v.i = readNum(sp, 4);
		Rf[i] = v.f;
		sp += 4;
	}
}
