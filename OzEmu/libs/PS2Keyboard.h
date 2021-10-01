unsigned char get_scan_code();
int kbd_begin();
static void clean_up(int x);

class PS2Keyboard {
private:
	int ch = -1;
	
public:
	bool available(){
		ch = get_scan_code();
		return ch >= 0;
	}

	unsigned char read(){
		return ch;
	}
};
