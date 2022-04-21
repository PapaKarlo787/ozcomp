extern "C" unsigned char get_scan_code();
extern "C" unsigned char get_mice_code();
extern "C" int kbd_begin();
extern "C" void clean_up();

class PS2 {
private:
	int16_t dev = -1;
	uint8_t (*devs[2]) (void) { get_scan_code, get_mice_code };

public:
	PS2(uint8_t _, uint8_t __) {
		kbd_begin();
	}

	void write(uint8_t value) {
		dev = value;
	}

	uint8_t read() {
		uint8_t res = devs[dev]();
		if (dev == 0 && res == 1) {
			endwin();
			clean_up();
			beeper.close();
			exit(0);
		}
		dev = -1;
		return res;
	}
};
