extern "C" unsigned char get_scan_code();
extern "C" int kbd_begin();
extern "C" void clean_up(int x);

class PS2Keyboard {
private:
	int ch = -1;

public:
	void begin(int _, int __){
		kbd_begin();
	}

	bool available(){
		ch = get_scan_code();
		return ch >= 0;
	}

	unsigned char read(){
		int x = get_scan_code();
		return x;
	}
};
