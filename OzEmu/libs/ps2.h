extern "C" unsigned char get_scan_code();
extern "C" unsigned char get_mice_code();
extern "C" int kbd_begin();
extern "C" void clean_up();

#define PS2DEVSCOUNT 4

uint8_t PS2zero() {
	return 0;
}

class PS2 {
private:
	int16_t dev = -1;
	uint8_t count = 0;
	uint8_t (*devs[PS2DEVSCOUNT]) (void) { get_scan_code, get_mice_code, PS2zero, PS2zero };
	uint8_t counts[PS2DEVSCOUNT] { 1, 3, 1, 1 };

public:
	PS2(uint8_t _, uint8_t __) {}
	
	void begin() {
		kbd_begin();
	}

	void write(uint8_t value) {
		if (value >= PS2DEVSCOUNT || count) return;
		dev = value;
		count = counts[value];
	}

	uint8_t read() {
		if (dev < 0) return 0;
		uint8_t res = devs[dev]();
		bool kbd_exit = dev == 0 && res == 1;
		bool mice_exit = dev == 1 && count == 3 && (res & 7) == 7;
		if (kbd_exit) {
			endwin();
			clean_up();
			beeper.close();
			exit(0);
		}
		count -= 1;
		if (!count) dev = -1;
		return res;
	}
};
