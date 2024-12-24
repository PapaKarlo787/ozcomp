void point_rr() {
	readRegisters();
	setFlags(lcd.set_point(R[r1], R[r2]));
}

void point_cc() {
	setFlags(lcd.set_point(read_(), read_()));
}

void circle_r() {
	readRegisters();
	int16_t r = R[r1];
	readRegisters();
	lcd.draw_circle(R[r1], R[r2], r);
}

void circle_rc() {
	int16_t r = read_();
	readRegisters();
	lcd.draw_circle(R[r1], R[r2], r);
}

void circle_c() {
	int16_t r = read_();
	lcd.draw_circle(read_(), read_(), r);
}

void line_rc() {
	readRegisters();
	lcd.draw_line(R[r1], R[r2], read_() + R[r2], read_() + R[r1]);
}

void line_c() {
	lcd.draw_line(read_(), read_(), read_(), read_());
}

void line_r() {
	readRegisters();
	int16_t x = R[r1];
	int16_t y = R[r2];
	readRegisters();
	lcd.draw_line(R[r1], R[r2], x, y);
}

void rect_rc() {
	int16_t y = read_();
	int16_t x = read_();
	readRegisters();
	lcd.draw_line(R[r1], R[r2], R[r1]+x, R[r2]);
	lcd.draw_line(R[r1], R[r2], R[r1], R[r2]+y);
	lcd.draw_line(R[r1]+x, R[r2]+y, R[r1], R[r2]+y);
	lcd.draw_line(R[r1]+x, R[r2]+y, R[r1]+x, R[r2]);
}

void rect_r() {
	readRegisters();
	int16_t x = R[r1];
	int16_t y = R[r2];
	readRegisters();
	lcd.draw_line(R[r1], R[r2], x, R[r2]);
	lcd.draw_line(R[r1], R[r2], R[r1], y);
	lcd.draw_line(x, y, R[r1], y);
	lcd.draw_line(x, y, x, R[r2]);
}

void rect_c() {
	int16_t y2 = read_();
	int16_t x2 = read_();
	int16_t y1 = read_();
	int16_t x1 = read_();
	lcd.draw_line(x1, y1, x2, y1);
	lcd.draw_line(x1, y1, x1, y2);
	lcd.draw_line(x2, y2, x2, y1);
	lcd.draw_line(x2, y2, x1, y2);
}
