

unsigned char screen_buffer[504];


void draw_screen() {
	lcd.setCursor(0, 0);
	for(unsigned int i = 0; i < 504; i++)
		lcd.send(HIGH, screen_buffer[i]);
}

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
	unsigned int z = readNum(4);
	draw_circle(readNum(1), readNum(1), *(float*)&z);
}

void line_c() {
	draw_line(readNum(1), readNum(1), readNum(1), readNum(1));
}

void line_r() {
	readRegisters();
	char x = R[r1];
	char y = R[r2];
	readRegisters();
	draw_line(x, y, R[r1], R[r2]);
}

void draw_line(char x0, char y0, char x1, char y1) {
	//std::cout << (int)x1 << (int)y1 <<std::endl;
	char A = y1 - y0;
	char B = x0 - x1;
	char signa = A < 0 ? -1 : 1;
	char signb = B < 0 ? -1 : 1;
	set_point(x0, y0);
	char x = x0, y = y0, f = 0;
	if (abs(A) < abs(B)) _draw_line(A, B, x0, y0, x1, y1);
	else _draw_line(B, A, y0, x0, y1, x1);
}


void _draw_line(char A, char B, char x0, char y0, char x1, char y1){
	char signa = A < 0 ? -1 : 1;
	char signb = B < 0 ? -1 : 1;
	char f = 0;
	do {
		if ((f += A*signa) > 0)
		{
			f -= B*signb;
			y0 += signa;
		}
		set_point((x0 -= signb), y0);
	} while (x0 != x1 || y0 != y1);
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
	x = ((int)x % 84 + 84) % 84;
	y = ((int)y % 48 + 48) % 48;
	int n = (int)(y / 8) * 84 + x;
	screen_buffer[n] |= (unsigned char)pow(2, (int)y % 8);
	lcd.setCursor(x, y/8);
	lcd.send(HIGH, screen_buffer[n]);
}
