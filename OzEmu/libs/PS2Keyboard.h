#include <curses.h>

class PS2Keyboard {
private:
	int ch = -1;
	
public:
	void begin(unsigned int _, unsigned int __){
		initscr();
		noecho();
		nodelay(stdscr, true);
	}
	
	bool available(){
		ch = getch();
		return ch >= 0;
	}

	unsigned char read(){
		if (ch == 27)
			if (ch = getch() == 91)
				ch = getch();
		auto result = ch % 256;
		ch = -1;
		return result;
	}
};
