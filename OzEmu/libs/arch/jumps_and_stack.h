void jmp() {
	ip = (unsigned int)readNum(4);
}

void jmp_c() {
	char f = (char)readNum(1);
	if((f & flags) != 0)
		jmp();
	else
		ip += 4;
}

void call() {
	unsigned int old_ip = ip + 4;
	ip = readNum(4);
	for (char i = 0; i < 4; i++) { 
		unsigned char t[1];
		t[0] = old_ip % 256;
		old_ip /= 256;
		write(sp-=1, t, 1);
	}
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
