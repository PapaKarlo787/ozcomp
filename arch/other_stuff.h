void to_int() {
	readRegisters();
	setFlags(Rf[r1] = (uint32_t)Rf[r1]);
}

void del_r() {
	delay(R[read_()]);
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
	uint8_t n = 0;
	for (uint32_t i = readNum(); i > 0; i--)
		write_(R[r1] + i - 1, &n, 1);	
}

void mzer_c() {
	uint32_t poi = readNum();
	uint8_t n = 0;
	for (uint32_t i = readNum(); i > 0; i--)
		write_(poi + i - 1, &n, 1);
}

void set_time() {
	timeUnix.settimeUnix(R[read_()]);
}

void get_time() {
	R[read_()] = timeUnix.gettimeUnix();
}

void mls() {
	R[12] = millis();
}

void mcs() {
	R[12] = micros();
}

void rgb_c() {
	analogWrite(Rp, read_());
	analogWrite(Gp, read_());
	analogWrite(Bp, read_());
}

void rgb_r() {
	readRegisters();
	analogWrite(Rp, R[r1]);
	analogWrite(Gp, R[r2]);
	analogWrite(Bp, R[read_()]);
}

void load_reg() {
	readRegisters();
	R[r1] = S[r2];
}

void store_reg() {
	readRegisters();
	S[r1] = R[r2];
}

void mcp() {
	readRegisters();
	sd_raw_copy_sector(R[r2], R[r1]);
}

void scond() {
	uint8_t	x = read_();
	flags |= (uint32_t)1 << (x + 16);
	if (!x) lcd.setColor(1);
}

void rcond() {
	uint8_t	x = read_();
	flags &= ~((uint32_t)1 << (x + 16));
	if (!x) lcd.setColor(0);
}
