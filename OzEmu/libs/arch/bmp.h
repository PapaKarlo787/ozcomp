void bmp_rc() {
	readRegisters();
	draw_bmp(R[r1], readNum(1));
}

void bmp_cr() {
	char x = readNum(1);
	readRegisters();
	draw_bmp(x, R[r1]);
}

void bmp_rr() {
	readRegisters();
	draw_bmp(R[r1], R[r2]);	
}

void bmp_cc() {
	draw_bmp(readNum(1), readNum(1));
}

void draw_bmp(char x, char y) {
	unsigned int t = ip;
	ip = readNum(4);
	char sx = readNum(1);
	char sy = readNum(1);
	char shift = (8 - y % 8) % 8;
	bool intersected = false;
	if (y < 0) y -= 8;
	y = y / 8 + (y % 8 ? 1 : 0);
	int start = y * 84 + x;
	for (char i = 0; i < sy; i++) {
		if (i + y > -1 && i + y < 7){
			for (char l = 0; l < sx; l++) {
				if (l + x > -1 && l + x < 84) {
					int c = readNum(1) * 256 >> shift;
					if (i + y > 0)
						intersected |= set_data(c % 256, start+l-84);
					if (i + y < 6)
						intersected |= set_data((c / 256) % 256, start+l);
				} else ip++;
			}
		} else ip += sx;
		start += 84;
	}
	ip = t+4;
	if (intersected)
		flags |= (int)pow(2, 3);
	else
		flags &= 255 - (int)pow(2, 3);
}

bool set_data(unsigned char data, int i) {
	unsigned char last = screen_buffer[i];
	screen_buffer[i] |= data;
	return last + data != screen_buffer[i];
}
