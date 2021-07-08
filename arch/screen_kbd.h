void get_key() {
	setFlags(R[15] = keyboard.available() ? keyboard.read() : 0);
}

void print_line_c() {
	uint32_t temp = readNum();
	while (uint8_t c = read_(temp++))
		lcd.write(c);
}

void print_line_r() {
	readRegisters();
	uint32_t temp = R[r0];
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
	lcd.setCursor(read_(), read_());
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
