void get_key() {
	setFlags(R[15] = keyboard.available() ? keyboard.read() : 0);
}

void print_line() {
	uint8_t temp = ip;
	ip = readNum(4);
	while (uint8_t c = read_())
		lcd.write(c);
	ip = temp + 4;
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
	lcd.send(HIGH, readNum(1));
}

void set_cursor() {
	lcd.setCursor(readNum(1), readNum(1));
}

void set_cursor_r() {
	readRegisters();
	lcd.setCursor(R[r1], R[r2]);
}

void clear_screen() {
	lcd.setCursor(0, 0);
	for(uint16_t i = 0 ; i < 504; i++)
		lcd.send(HIGH, screen_buffer[i] = 0);
}

void draw_screen() {
	lcd.setCursor(0, 0);
	for(uint16_t i = 0; i < 504; i++)
		lcd.send(HIGH, screen_buffer[i]);
}
