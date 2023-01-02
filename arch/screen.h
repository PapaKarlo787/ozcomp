void print_line_c() {
	uint32_t temp = readNum();
	while (uint8_t c = read_(temp++))
		lcd.write(c);
}

void print_line_r() {
	readRegisters();
	uint32_t temp = R[r1];
	while (uint8_t c = read_(temp++))
		lcd.write(c);
}

void print_num() {
	readRegisters();
	lcd.print(Rf[r1]);
}

void print_int() {
	readRegisters();
	lcd.print(R[r1]);
}

void snd_r() {
	readRegisters();
	lcd.send(HIGH, R[r1]);
}

void snd_c() {
	lcd.send(HIGH, read_());
}

void set_cursor() {
	uint8_t x = read_();
	lcd.setCursor(x, read_());
}

void set_cursor_r() {
	readRegisters();
	lcd.setCursor(R[r1], R[r2]);
}

void clear_screen() {
	lcd.clear();
}

void rpix_c() {
	readRegisters();
	lcd.reverse(R[r1], R[r1] + read_());
}

void rpix_r() {
	readRegisters();
	lcd.reverse(R[r1], R[r1] + R[r2]);
}

void scond() {
	flags |= (uint32_t)1 << (read_()+16);
}

void rcond() {
	flags &= ~((uint32_t)1 << read_()+16);
}
