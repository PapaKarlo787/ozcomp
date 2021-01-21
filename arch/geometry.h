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

void circle_c() {
	uint32_t z = readNum(4);
	draw_circle(readNum(1), readNum(1), *(float*)&z);
}

void line_c() {
	draw_line(readNum(1), readNum(1), readNum(1), readNum(1));
}

void line_r() {
	readRegisters();
	uint32_t x = R[r1];
	uint32_t y = R[r2];
	readRegisters();
	draw_line(x, y, R[r1], R[r2]);
}

void draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
	uint32_t A = y1 - y0;
	uint32_t B = x0 - x1;
	set_point(x0, y0);
	if (abs(A) < abs(B)) _draw_line(A, B, x0, y0, x1, y1);
	else _draw_line(B, A, y0, x0, y1, x1);
}


void _draw_line(uint32_t A, uint32_t B, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1){
	uint32_t signa = A < 0 ? -1 : 1;
	uint32_t signb = B < 0 ? -1 : 1;
	uint32_t f = 0;
	do {
		if ((f += A*signa) > 0)
		{
			f -= B*signb;
			y0 += signa;
		}
		set_point((x0 -= signb), y0);
	} while (x0 != x1 || y0 != y1);
}

void draw_circle(uint32_t X1, uint32_t Y1, float R) {
	uint32_t x = 0;
	uint32_t y = R;
	uint32_t delta = 1 - 2 * R;
	uint32_t error = 0;
	while (y >= 0) {
		set_point(X1 + x, Y1 + y);
		set_point(X1 + x, Y1 - y);
		set_point(X1 - x, Y1 + y);
		set_point(X1 - x, Y1 - y);
		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0)) {
			delta += 2 * ++x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0)) {
			delta -= 2 * --y + 1;
			continue;
		}
		delta += 2 * (++x - --y);
	}
}

void set_point(float x, float y) {
	x = ((uint32_t)x % 84 + 84) % 84;
	y = ((uint32_t)y % 48 + 48) % 48;
	uint32_t n = (uint32_t)(y / 8) * 84 + x;
	screen_buffer[n] |= (uint32_t)pow(2, (uint32_t)y % 8);
	lcd.setCursor(x, y/8);
	lcd.send(HIGH, screen_buffer[n]);
}
