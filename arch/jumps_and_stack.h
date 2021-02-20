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
	write(sp-=4, (uint8_t*)&ip, 4);
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
	write(sp -= 4, (unsigned char*)&(R[r1]), 4);
}

void pop() {
	readRegisters();
	unsigned int temp = ip;
	ip = sp;
	unsigned int x = readNum(4);
	R[r1] = *(float*)&x;
	ip = temp;
	sp += 4;
}
