class Lpstack {
	uint8_t* data;
	uint16_t head, tail, size;

public:
	Lpstack(uint16_t size_) {
		data = new uint8_t[size_];
		head = tail = 0;
		size = size_;
	}
	
	void push(uint8_t value) {
		data[tail] = value;
		inc(tail);
		if (tail == head)
			inc(head);
	}
	
	void inc(uint16_t &value) {
		value = (value+1) % size;
	}
	
	uint8_t pop() {
		if (head == tail) return 0;
		uint8_t res = data[head];
		inc(head);
		return res;
	}
};
