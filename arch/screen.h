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
	lcd.send_data(R[r1]);
}

void snd_c() {
	lcd.send_data(read_());
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

void rev() {
	lcd.reverse();
}

void sts_r() {
	sd_raw_write(R[read_()], lcd.screen_buffer, bufsize);
}

void sts_c() {
	sd_raw_write(readNum(), lcd.screen_buffer, bufsize);
}

void lds_r() {
	sd_raw_read(R[read_()], lcd.screen_buffer, bufsize);
	lcd.rst();
}

void lds_c() {
	sd_raw_read(readNum(), lcd.screen_buffer, bufsize);
	lcd.rst();
}
