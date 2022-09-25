#include <ncurses.h>
#include <string>
#include <sys/ioctl.h>
#include "charset.cpp"

class PCD8544{
private:
	WINDOW *window;

public:
	uint16_t cursor = 0;
	uint8_t screen_buffer[504];

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
		cursor = x + y * 84;
	}

	void write(uint8_t c){
		if (c > 127) c = 127;
		for (uint8_t i = 0; i < 5; i++)
			send(0, screen_buffer[cursor] = charset[c][i]);
		send(0, screen_buffer[cursor] = 0);
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
		uint8_t x = cursor % 84;
		uint8_t y = cursor / 84 * 8;
		cursor = (cursor + 1) % 504;
		for (unsigned int i = 0; i < 8; i++)
			mvwaddch(window, y+i, x, (num >> i) & 1 ? 219 : ' ');
		wrefresh(window);
	}
};
