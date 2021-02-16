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
	unsigned int z = readNum(4);
	readRegisters();
	draw_circle(R[r1], R[r2], *(float*)&z);
}

void circle_c() {
	unsigned int z = readNum(4);
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
	char x = R[r1];
	char y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, y);
}

void rect_rc() {
	char y = readNum(1);
	char x = readNum(1);
	readRegisters();
	draw_line(R[r1], R[r2], R[r1]+x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1], R[r2]+y);
	draw_line(R[r1]+x, R[r2]+y, R[r1]+x, R[r2]);
}

void rect_r() {
	readRegisters();
	char x = R[r1];
	char y = R[r2];
	readRegisters();
	draw_line(R[r1], R[r2], x, R[r2]);
	draw_line(R[r1], R[r2], R[r1], y);
	draw_line(x, y, R[r1], y);
	draw_line(x, y, x, R[r2]);
}

void rect_c() {
	char y2 = readNum(1);
	char x2 = readNum(1);
	char y1 = readNum(1);
	char x1 = readNum(1);
	draw_line(x1, y1, x2, y1);
	draw_line(x1, y1, x1, y2);
	draw_line(x2, y2, x2, y1);
	draw_line(x2, y2, x1, y2);
}

void draw_line(char x0, char y0, char x1, char y1) {
	int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
	int sx = (x1 >= x0) ? (1) : (-1);
	int sy = (y1 >= y0) ? (1) : (-1);
	set_point(0, 0);
	if (dy < dx)
		_draw_line(x0 + sx, y0, dy, dx, 0, 0, sx, sy);
	else
		_draw_line(x0, y0 + sy, dx, dy, sx, sy, 0, 0);
}

void _draw_line(int x, int y, int dx, int dy, int sx, int sy, int sxx, int syy) {
	int d = (dx << 1) - dy;
	int d1 = dx << 1;
	int d2 = (dx - dy) << 1;
	for (int i = 1; i <= dy; i++)
	{
		if (d > 0)
		{
			d += d2;
			x += sx;
			y += syy;
		}
		else
			d += d1;
		set_point(x, y);
		y += sy;
		x += sxx;
	}
}

void draw_circle(char X1, char Y1, float R) {
	char x = 0;
	char y = R;
	int delta = 1 - 2 * R;
	int error = 0;
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

void set_point(int x, int y) {
	if (x > 83 || x < 0 || y > 47 || y < 0)
		return;
	int n = (int)(y / 8) * 84 + x;
	if (color)
		screen_buffer[n] |= (1 << y % 8);
	else
		screen_buffer[n] &= ~(1 << y % 8);
	lcd.setCursor(x, y/8);
	lcd.send(HIGH, screen_buffer[n]);
}


void scol() {
	color = readNum(1) ? 255 : 0;
}
