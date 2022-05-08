void set_point(int16_t x, int16_t y) {
	if (x > 83 || x < 0 || y > 47 || y < 0)
		return;
	uint16_t n = y / 8 * 84 + x;
	uint8_t tmp = screen_buffer[n]; 
	if (flags & (1 << 16))
		screen_buffer[n] |= (1 << y % 8);
	else
		screen_buffer[n] &= ~(1 << y % 8);
	lcd.setCursor(x, y/8);
	lcd.send(HIGH, screen_buffer[n]);
	flags = tmp == screen_buffer[n] ? flags | 8 : flags & ~8;
}

void _draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy) {
	int16_t d = (dx << 1) - dy;
	int16_t d1 = dx << 1;
	int16_t d2 = (dx - dy) << 1;
	for (uint16_t i = 1; i <= dy; i++) {
		if (d > 0) {
			d += d2;
			x += sx;
			y += syy;
		} else d += d1;
		set_point(x, y);
		y += sy;
		x += sxx;
	}
}

void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	uint16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	uint16_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
	int8_t sx = (x1 >= x0) ? (1) : (-1);
	int8_t sy = (y1 >= y0) ? (1) : (-1);
	set_point(x0, y0);
	if (dy < dx)
		_draw_line(x0 + sx, y0, dy, dx, 0, 0, sx, sy);
	else
		_draw_line(x0, y0 + sy, dx, dy, sx, sy, 0, 0);
}

void draw_circle(int16_t X1, int16_t Y1, int16_t y) {
	int16_t x = 0;
	int16_t delta = 1 - 2 * y;
	int16_t error = 0;
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

void point_rr() {
	readRegisters();
	set_point(R[r1], R[r2]);
}

void point_cc() {
	set_point(read_(), read_());
}

void circle_r() {
	readRegisters();
	int16_t r = R[r1];
	readRegisters();
	draw_circle(R[r1], R[r2], r);
}

void circle_rc() {
	int16_t r = read_();
	readRegisters();
	draw_circle(R[r1], R[r2], r);
}

void circle_c() {
	int16_t r = read_();
	draw_circle(read_(), read_(), r);
}

void line_rc() {
	readRegisters();
	draw_line(R[r1], R[r2], read_()+R[r2], read_()+R[r1]);
}

void line_c() {
	draw_line(read_(), read_(), read_(), read_());
}

void line_r() {
	readRegisters();
	int16_t x = R[r1];
	int16_t y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, y);
}

void rect_rc() {
	int16_t y = read_();
	int16_t x = read_();
	readRegisters();
	draw_line(R[r1], R[r2], R[r1]+x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1]+x, R[r2]);
}

void rect_r() {
	readRegisters();
	int16_t x = R[r1];
	int16_t y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], y);
	draw_line(x, y, R[r1], y);
	draw_line(x, y, x, R[r2]);
}

void rect_c() {
	int16_t y2 = read_();
	int16_t x2 = read_();
	int16_t y1 = read_();
	int16_t x1 = read_();
	draw_line(x1, y1, x2, y1);
	draw_line(x1, y1, x1, y2);
	draw_line(x2, y2, x2, y1);
	draw_line(x2, y2, x1, y2);
}

void scol() {
	flags &= ~(1 << 16);
	flags |= (read_() ? 1 : 0) << 16;
}
