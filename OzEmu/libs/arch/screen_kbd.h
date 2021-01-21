void get_key() {
	R[15] = keyboard.available() ? keyboard.read() : 0;
}

void print_line() {
	unsigned int temp = ip;
	ip = readNum(4);
	while (char c = read())
		lcd.write(c);
	ip = temp + 4;
}

void print_num() {
	readRegisters();
	lcd.print(R[r1]);
}

void print_int() {
	readRegisters();
	lcd.print((int)R[r1]);
}

void snd_r() {
	readRegisters();
	lcd.send(HIGH, (char)R[r1]);
}

void snd_c() {
	lcd.send(HIGH, (char)readNum(1));
}

void set_cursor() {
	lcd.setCursor((char)readNum(1), (char)readNum(1));
}

void set_cursor_r() {
	readRegisters();
	lcd.setCursor((unsigned int)R[r1], (unsigned int)R[r2]);
}

void clear_screen() {
	for(int i = 0 ; i < 504; i++) {
		screen_buffer[i] = 0;
		lcd.send(HIGH, 0);
	}
}
