void jmp() {
	ip = readNum(4);
}

void jmp_c() {
	uint8_t f = readNum(1);
	if((f & flags) != 0)
		jmp();
	else
		ip += 4;
}

void call() {
	uint32_t old_ip = ip + 4;
	write_(sp-=4, (uint8_t*)&old_ip, 4);
	ip = readNum(4);
}

void ret() {
	ip = sp;
	ip = readNum(4);
	sp += 4;
}

void lp() {
	ip = R[14]-- > 0 ? readNum(4) : ip + 4;
}

void push() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(R[r1]), 4);
}

void pop() {
	readRegisters();
	uint32_t temp = ip;
	ip = sp;
	R[r1] = readNum(4);
	ip = temp;
	sp += 4;
}

void fpush() {
	readRegisters();
	write_(sp -= 4, (uint8_t*)&(Rf[r1]), 4);
}

void fpop() {
	readRegisters();
	uint32_t temp = ip;
	ip = sp;
	uint32_t x = readNum(4);
	Rf[r1] = *(float*)&x;
	ip = temp;
	sp += 4;
}
