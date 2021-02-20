void to_int() {
	readRegisters();
	setFlags(Rf[r1] = (uint32_t)Rf[r1]);
}

void del_r() {
	readRegisters();
	delay(R[r1]);
}

void del_c() {
	delay(readNum(4));
}

void rnd(){
	R[13] = random(1024);
}
