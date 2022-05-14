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
	lcd.setCursor(0, 0);
	for(uint16_t i = 0 ; i < 504; i++)
		lcd.send(HIGH, screen_buffer[i] = 0);
}

void draw_screen() {
	lcd.setCursor(0, 0);
	for(uint16_t i = 0; i < 504; i++)
		lcd.send(HIGH, screen_buffer[i]);
}

void rpix_c() {
	readRegisters();
	if (R[r1] >= 504) return;
	lcd.setCursor(R[r1] % 84, R[r1] / 84);
	uint16_t n = R[r1] + read_();
	for(uint16_t i = R[r1]; i < n; i++)
		lcd.send(HIGH, screen_buffer[i] ^= 0xff);
}

void rpix_r() {
	readRegisters();
	if (R[r1] >= 504) return;
	lcd.setCursor(R[r1] % 84, R[r1] / 84);
	uint16_t n = R[r1] + R[r2];
	for(uint16_t i = R[r1]; i < n; i++)
		lcd.send(HIGH, screen_buffer[i] ^= 0xff);
}

void scond() {
	flags &= 0xffff;
	flags |= ((uint16_t)read_()) << 16;
}
