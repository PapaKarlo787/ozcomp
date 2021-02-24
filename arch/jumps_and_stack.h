void jmp() {
	ip = readNum();
}

void jmp_c() {
	ip = (read_() & flags) != 0 ? readNum() : ip + 4;
}

void call() {
	uint32_t old_ip = ip + 4;
	write_(sp-=4, (uint8_t*)&old_ip, 4);
	ip = readNum();
}

void ret() {
	ip = sp;
	ip = readNum();
	sp += 4;
}

void lp() {
	ip = R[14]-- > 0 ? readNum() : ip + 4;
}

void push() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(R[r1]), 4);
}

void pop() {
	readRegisters();
	R[r1] = readNum(sp);
	sp += 4;
}

void fpush() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(Rf[r1]), 4);
}

void fpop() {
	readRegisters();
	uint32_t x = readNum(sp);
	Rf[r1] = *(float*)&x;
	sp += 4;
}
