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

void set_time() {
	readRegisters();
	timeUnix.settimeUnix(R[read_()]);
}

void get_time() {
	R[read_()] = timeUnix.gettimeUnix();
}

void mls(){
	R[12] = millis();
}

void mcs(){
	R[12] = micros();
}

void rgb_c(){
	analogWrite(Rp, read_());
	analogWrite(Gp, read_());
	analogWrite(Bp, read_());
}

void rgb_r(){
	readRegisters();
	analogWrite(Rp, R[r1]);
	analogWrite(Gp, R[r2]);
	analogWrite(Bp, R[read_()]);
}
