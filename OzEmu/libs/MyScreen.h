#include <ncurses.h>
#include <string>
#include <sys/ioctl.h>
#include "charset.cpp"

#define width 84
#define height 48
#define bufsize 504

class PCD8544{
private:
	WINDOW *window;

public:
	uint16_t cursor = 512;
	uint8_t screen_buffer[bufsize];

	void begin() {
		struct winsize size;
		uint16_t x, y;
		if (ioctl(0, TIOCGWINSZ, (char *) &size) < 0) {
			x = 0;
			y = 0;
		} else {
			x = (size.ws_col - 84) >> 1;
			y = (size.ws_row - 48) >> 1;
		}
		initscr();
		noecho();
		curs_set(0);
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		window = newwin(48, 84, y, x);
		wbkgd(window, COLOR_PAIR(1));
		wrefresh(window);
	}

	void setCursor(uint8_t x, uint8_t y){
		x %= 84;
		y %= 6;
		cursor = (cursor & 0xfe00) + x + y * 84;
	}
	
	void setColor(uint8_t c){
		cursor &= 511;
		cursor |= c << 9;
	}

	void write(uint8_t c){
		if (c > 127) c = 127;
		for (uint8_t i = 0; i < 5; i++)
			send(0, charset[c][i]);
		send(0, 0);
	}

	void print(float num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}
	
	void print(uint32_t num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}

	void send(uint32_t _, uint8_t num){
		uint8_t x = (cursor & 511) % 84;
		uint8_t y = (cursor & 511) / 84 * 8;
		screen_buffer[cursor & 511] = num;
		cursor = (cursor & 0xfe00) + ((cursor & 0x1ff) + 1) % bufsize;
		for (unsigned int i = 0; i < 8; i++)
			mvwaddch(window, y+i, x, (num >> i) & 1 ? 219 : ' ');
		wrefresh(window);
	}

	bool set_point(int16_t x, int16_t y) {
		if (x >= width || x < 0 || y >= height || y < 0)
			return 0;
		uint16_t n = y / 8 * width + x;
		uint8_t tmp = screen_buffer[n];
		setColored(n, 1 << y % 8);
		setCursor(x, y/8);
		send(HIGH, screen_buffer[n]);
		return tmp == screen_buffer[n];
	}

	void draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy) {
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
			draw_line(x0 + sx, y0, dy, dx, 0, 0, sx, sy);
		else
			draw_line(x0, y0 + sy, dx, dy, sx, sy, 0, 0);
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

	bool set_data(uint8_t data, uint16_t i) {
		uint8_t last = screen_buffer[i];
		setColored(i, data);
		setCursor(i % width, i / width);
		send(HIGH, screen_buffer[i]);
		return last + data != screen_buffer[i];
	}

	bool draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi) {
		uint8_t sx = reader();
		uint8_t sy = reader();
		uint8_t shift = (8 - y % 8) % 8;
		bool intersected = false;
		if (y < 0) y -= 8;
		y = y / 8 + (y % 8 ? 1 : 0);
		int start = y * width + x;
		for (uint8_t i = 0; i < sy; i++) {
			if (i + y > -1 && i + y < 7){
				for (uint8_t l = 0; l < sx; l++) {
					if (l + x > -1 && l + x < width) {
						uint16_t c = (uint16_t)reader() << (8 - shift);
						if (i + y > 0)
							intersected |= set_data(c & 0xff, start+l-width);
						if (i + y < 6)
							intersected |= set_data(c >> 8, start+l);
					} else (*poi)++;
				}
			} else (*poi) += sx;
			start += width;
		}
		return intersected;
	}

	void setColored(uint16_t i, uint8_t data) {
		if (cursor & 512)
			screen_buffer[i] |= data;
		else
			screen_buffer[i] &= ~data;
	}

	void clear() {
		setCursor(0, 0);
		for(uint16_t i = 0; i < bufsize; i++)
			send(HIGH, 0);
	}

	void reverse(uint16_t from, uint16_t to) {
		if (from >= bufsize) return;
		setCursor(from % width, from / width);
		for(uint16_t i = from; i < to; i++)
			send(HIGH, ~screen_buffer[i]);
	}
};
