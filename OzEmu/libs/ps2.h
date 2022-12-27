extern "C" unsigned char get_scan_code();
extern "C" int kbd_begin();
extern "C" void clean_up();

class PS2Keyboard {
public:
	PS2Keyboard() {}
	
	void begin() {
		kbd_begin();
	}

	void write(uint8_t value) { }
	uint8_t take() { return read(); }

	uint8_t read() {
		uint8_t res = get_scan_code();
		bool kbd_exit = res == 1;
		if (kbd_exit) {
			endwin();
			clean_up();
			beeper.close();
			exit(0);
		}
		return res;
	}
};
