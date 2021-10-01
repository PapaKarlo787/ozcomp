#include <ncurses.h>
#include <string>
#include "charset.cpp"

class PCD8544{
private:
	WINDOW *window;

public:
	uint16_t cursor = 0;
	uint8_t screen_buffer[504];

	void begin(){
		initscr();
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		setlocale(LC_ALL, "");
		window = newwin(48, 84, 0, 0);
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
			send(0, screen_buffer[cursor] = charset[c-32][i]);
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
			mvwaddch(window, y+i, x, (num >> i) & 1 ? 254 : ' ');
		wrefresh(window);
	}
};
