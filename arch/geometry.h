void point_rr() {
	readRegisters();
	set_point(R[r1], R[r2]);
}

void point_cc() {
	set_point(readNum(1), readNum(1));
}

void circle_r() {
	readRegisters();
	float r = R[r1];
	readRegisters();
	draw_circle(R[r1], R[r2], r);
}

void circle_rc() {
	uint32_t z = readNum(4);
	readRegisters();
	draw_circle(R[r1], R[r2], *(float*)&z);
}

void circle_c() {
	uint32_t z = readNum(4);
	draw_circle(readNum(1), readNum(1), *(float*)&z);
}

void line_rc() {
	readRegisters();
	draw_line(R[r1], R[r2], readNum(1)+R[r2], readNum(1)+R[r1]);
}

void line_c() {
	draw_line(readNum(1), readNum(1), readNum(1), readNum(1));
}

void line_r() {
	readRegisters();
	int8_t x = R[r1];
	int8_t y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, y);
}

void rect_rc() {
	int8_t y = readNum(1);
	int8_t x = readNum(1);
	readRegisters();
	draw_line(R[r1], R[r2], R[r1]+x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1]+x, R[r2]);
}

void rect_r() {
	readRegisters();
	int8_t x = R[r1];
	int8_t y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], y);
	draw_line(x, y, R[r1], y);
	draw_line(x, y, x, R[r2]);
}

void rect_c() {
	int8_t y2 = readNum(1);
	int8_t x2 = readNum(1);
	int8_t y1 = readNum(1);
	int8_t x1 = readNum(1);
	draw_line(x1, y1, x2, y1);
	draw_line(x1, y1, x1, y2);
	draw_line(x2, y2, x2, y1);
	draw_line(x2, y2, x1, y2);
}

void draw_line(char x1, char y1, char x2, char y2) {
  uint8_t deltaX = abs(x2 - x1);
  uint8_t deltaY = abs(y2 - y1);
  int8_t signX = x1 < x2 ? 1 : -1;
  int8_t signY = y1 < y2 ? 1 : -1;
  int8_t error = (deltaX - deltaY) * 2;
  set_point(x2, y2);
  
  while (pow(x1-x2, 2)+ pow(y1-y2, 2) > 1) {
    set_point(x1, y1);
    if (error > -deltaY) {
      error -= deltaY * 2;
      x1 += signX;
    }
    if (error < deltaX) {
      error += deltaX * 2;
      y1 += signY;
    }
  }

}

void draw_circle(int8_t X1, int8_t Y1, float R) {
	int8_t x = 0;
	int8_t y = R;
	int8_t delta = 1 - 2 * R;
	int8_t error = 0;
	while (y >= 0) {
		set_point(X1 + x, Y1 + y);
		set_point(X1 + x, Y1 - y);
		set_point(X1 - x, Y1 + y);
		set_point(X1 - x, Y1 - y);
		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
			delta += 2 * ++x + 1;
		else if ((delta > 0) && (error > 0))
			delta -= 2 * --y + 1;
		else
			delta += 2 * (++x - --y);
	}
}

void set_point(float x, float y) {
	if (x > 83 || x < 0 || y > 47 || y < 0)
		return;
	int n = (uint)(y / 8) * 84 + x;
	screen_buffer[n] |= (uint8_t)pow(2, (uint8_t)y % 8);
	lcd.setCursor(x, y/8);
	lcd.send(HIGH, screen_buffer[n]);
}


void scol() {
	color = readNum(1) ? 255 : 0;
}
