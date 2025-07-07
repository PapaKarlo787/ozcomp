#include <ncurses.h>
#include <string>
#include <sys/ioctl.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "charset.h"

#define SCREEN_width 84
#define SCREEN_height 48
#define SCREEN_bufsize 504

class Screen {
protected:
	uint8_t reversed = 0;

public:
	uint16_t cursor = 512;
	uint8_t screen_buffer[SCREEN_bufsize];
	
	~Screen() = default;

	virtual void begin() = 0;

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
			send_data(charset[c][i]);
		send_data(0);
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

	virtual void send_data(uint8_t num) = 0;
	virtual void update() = 0;

	bool set_point(int16_t x, int16_t y) {
		if (x >= SCREEN_width || x < 0 || y >= SCREEN_height || y < 0)
			return 0;
		uint16_t n = y / 8 * SCREEN_width + x;
		uint8_t tmp = screen_buffer[n];
		setColored(n, 1 << y % 8);
		setCursor(x, y/8);
		send_data(screen_buffer[n]);
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
		setCursor(i % SCREEN_width, i / SCREEN_width);
		send_data(screen_buffer[i]);
		return last + data != screen_buffer[i];
	}

	bool draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi) {
		uint8_t sx = reader();
		uint8_t sy = reader();
		uint8_t shift = (8 - y % 8) % 8;
		bool intersected = false;
		if (y < 0) y -= 8;
		y = y / 8 + (y % 8 ? 1 : 0);
		int start = y * SCREEN_width + x;
		for (uint8_t i = 0; i < sy; i++) {
			if (i + y > -1 && i + y < 7){
				for (uint8_t l = 0; l < sx; l++) {
					if (l + x > -1 && l + x < SCREEN_width) {
						uint16_t c = (uint16_t)reader() << (8 - shift);
						if (i + y > 0)
							intersected |= set_data(c & 0xff, start+l-SCREEN_width);
						if (i + y < 6)
							intersected |= set_data(c >> 8, start+l);
					} else (*poi)++;
				}
			} else (*poi) += sx;
			start += SCREEN_width;
		}
		return intersected;
	}

	void setColored(uint16_t i, uint8_t data) {
		if (cursor & 512)
			screen_buffer[i] |= data;
		else
			screen_buffer[i] &= ~data;
	}

	virtual void rst() = 0;

	void clear() {
		setCursor(0, 0);
		for(uint16_t i = 0; i < SCREEN_bufsize; i++)
			send_data(0);
	}

	void reverse(uint16_t from, uint16_t to) {
		if (from >= SCREEN_bufsize) return;
		setCursor(from % SCREEN_width, from / SCREEN_width);
		for(uint16_t i = from; i < to; i++)
			send_data(~screen_buffer[i]);
	}

	void reverse() {
		reversed ^= 0xff;
		for (uint16_t i = 0; i < SCREEN_bufsize; i++)
			send_data(screen_buffer[cursor & 511]);
	}
};

class GraphicsScreen: public Screen {
public:
	sf::RenderWindow window {sf::VideoMode(SCREEN_width * 10, SCREEN_height * 10), "Comp"};
	std::vector<sf::RectangleShape> pixels;
	void begin() override {
		for (int y = 0; y < SCREEN_height * 8; y++) {
			for (int x = 0; x < SCREEN_width; x++) {
				sf::RectangleShape p(sf::Vector2f(10, 10));
				p.setPosition(x * 10, y * 10);
				p.setFillColor(sf::Color(255, 0, 0));
				pixels.push_back(p);
				window.draw(p);
			}
		}
		window.display();
	}
	
	void update() override {
		sf::Event event;
		window.pollEvent(event);
		if (event.type==sf::Event::Closed)
			window.close();
	}
	
	void rst() override {
		setCursor(0, 0);
		for(uint16_t i = 0; i < SCREEN_bufsize; i++) {
			uint16_t x = i % SCREEN_width;
			uint16_t y = i / SCREEN_width * 8;
			for (uint16_t l = 0; l < 8; l++) {
				auto c = ((screen_buffer[i] >> l) ^ reversed) & 1 ? 255 : 0;
				uint16_t z = (y + l) * SCREEN_width + x;
				pixels[z].setFillColor(sf::Color(c, c, c));
				window.draw(pixels[z]);
			}
		}
		window.display();
	}
	
	void send_data(uint8_t num) override {
		uint16_t i = (cursor & 511);
		screen_buffer[cursor & 511] = num;
		cursor = (cursor & 0xfe00) + (i + 1) % SCREEN_bufsize;
		uint16_t x = i % SCREEN_width;
		uint16_t y = i / SCREEN_width * 8;
		for (uint16_t l = 0; l < 8; l++) {
			auto c = ((num >> l) ^ reversed) & 1 ? 255 : 0;
			uint16_t z = (y + l) * SCREEN_width + x;
			pixels[z].setFillColor(sf::Color(c, c, c));
			window.draw(pixels[z]);
		}
		window.display();
	}
};

class ConsoleScreen: public Screen {
private:
	WINDOW *window;

public:
	~ConsoleScreen() {
		endwin();
	}
	
	void begin() override {
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
	
	void update() override { }
	
	void rst() override {
		setCursor(0, 0);
		for(uint16_t i = 0; i < SCREEN_bufsize; i++) {
			uint8_t x = (i & 511) % 84;
			uint8_t y = (i & 511) / 84 * 8;
			for (uint8_t l = 0; l < 8; l++)
				mvwaddch(window, y+l, x, ((screen_buffer[i] >> l) ^ reversed) & 1 ? 219 : ' ');
		}
		wrefresh(window);
	}
	
	void send_data(uint8_t num) override {
		uint8_t x = (cursor & 511) % 84;
		uint8_t y = (cursor & 511) / 84 * 8;
		screen_buffer[cursor & 511] = num;
		cursor = (cursor & 0xfe00) + ((cursor & 0x1ff) + 1) % SCREEN_bufsize;
		for (uint8_t i = 0; i < 8; i++)
			mvwaddch(window, y+i, x, ((num >> i) ^ reversed) & 1 ? 219 : ' ');
		wrefresh(window);
	}
};
