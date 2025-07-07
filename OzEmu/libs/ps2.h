#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

class Queue {
public:
	Queue(uint16_t size) {
		data = (uint8_t*)malloc(size);
		this->size = size;
	}
	
	~Queue() {
		free(data);
	}
	
	void enqueue(uint8_t c) {
		uint16_t next = (head + 1) % size;
		if (next != tail) {
			data[head] = c;
			head = next;
		}
	}
	
	uint8_t dequeue() {
		if (head != tail) {
			uint8_t res = data[tail];
			tail = (tail + 1) % size;
			return res;
		}
		return 0;
	}
private:
	uint8_t* data;
	uint16_t head, tail, size;
};

class PS2 {
public:
	PS2(int size, int x, int y) : queue(size) { }
	~PS2() = default;
	
	virtual void begin() = 0;
	
	void readBit() {
		while (1) {
			if (::read(fd, (char*)&ev, sizeof ev) != sizeof ev) continue;
			if (ev.type == EV_MSC) {
				last_sc = ev.value & 0x7F;
			}
			else if (ev.type == EV_KEY) {
				queue.enqueue(ev.value ? last_sc : (last_sc | 0x80));
			}
		}
	};

	void write(uint8_t value) { }
	uint8_t take() { 
		uint8_t res = queue.dequeue();
		if (res == 1) exit(0);
		return res;
	}

	uint8_t read() {
		uint8_t res = 0;
		if (res == 1) {
			exit(0);
		}
		return res;
	}
protected:
	int fd;
	struct input_event ev;
	unsigned int last_sc;
	Queue queue;
};

class PS2Keyboard: public PS2 {
public:
	PS2Keyboard(uint8_t size, uint8_t clk, uint8_t dat): PS2(size, clk, dat) {}

	void begin() override {
		if ((fd = open("/dev/input/event2", O_RDONLY)) < 0) {
			perror("open");
			exit(1);
		}
	}
};

class PS2Mouse: public PS2 {
public:
	PS2Mouse(uint8_t size, uint8_t clk, uint8_t dat): PS2(size, clk, dat) {}
	
	void begin() override {
		if ((fd = open("/dev/input/mice", O_RDONLY)) < 0) { 
			perror("open");
			exit(1);
		}
	}
};
