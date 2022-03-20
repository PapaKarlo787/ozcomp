extern "C" unsigned char get_scan_code();
extern "C" int kbd_begin();
extern "C" void clean_up();
#define BUFFER_SIZE 20
#include <thread>

class PS2Keyboard {
private:
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head, tail;
	thread thr;

	static void interrupt_reader(PS2Keyboard* kbd) {
		while(1) {
			uint8_t x = get_scan_code();
			if (x) {
				if (x == 1) {
					endwin();
					clean_up();
					beeper.close();
				}
				(kbd->buffer)[kbd->head] = x;
				kbd->head = (kbd->head + 1) % BUFFER_SIZE;
			}
		}
	}

public:
	void begin(int _, int __){
		head = 0;
		tail = 0;
		kbd_begin();
		thr = thread(interrupt_reader, this);
	}

	bool available(){
		return tail != head;
	}

	uint8_t read(){
		if (tail == head) return 0;
		uint8_t res = buffer[tail];
		if (++tail >= BUFFER_SIZE) tail = 0;
		return res;
	}
};
