class Queue {
public:
	Queue(uint8_t _size) {
		head = tail = 0;
		size = _size;
		data = (uint8_t*)malloc(size);
	}
	
	uint8_t enqueue(uint8_t val) {
		uint8_t next = (head + 1) % size;
		if (next != tail) {
			data[head] = val;
			head = next;
			return 0;
		}
		return -1;
	}
	
	uint8_t dequeue(uint8_t* val) {
		if (tail == head) return -1;
		*val = data[tail];
		if (++tail >= size) tail = 0;
		return 0;
	}
	
private:
	uint8_t head, tail, size;
	uint8_t* data;
};

class PS2 {
public:
	PS2(uint8_t size, uint8_t clk, uint8_t dat): 
	queue(size), 
	dat(dat), 
	clk(clk),
	bitcount(0),
	incoming(0),
	prev_ms(0),
	is_init(0)
	{ }

    virtual void begin();
    
    void readBit() {
		if (digitalRead(clk)) return;
		
		uint32_t now_ms;
		uint8_t n, val;
		
		val = digitalRead(dat);
		now_ms = millis();
		
		if (now_ms - prev_ms > 250) {
			bitcount = 0;
			incoming = 0;
		}
		
		prev_ms = now_ms;
		n = bitcount - 1;
		
		if (n < 8) {
			incoming |= (val << n);
		}
		
		if (++bitcount == 11) {
			queue.enqueue(incoming);
			bitcount = 0;
			incoming = 0;
			if (!is_init) begin();
		}
	}

    uint8_t take() {
		uint8_t res = 0;
		queue.dequeue(&res);
		return res;
	}

	uint8_t read() {
		uint8_t data = 0x00;
		// start clock
		gohi(clk);
		gohi(dat);
		wait(clk, LOW, 10);

		wait(clk, HIGH, 10);	// eat start bit
		for (uint8_t i = 0; i < 8; i++) {
			wait(clk, LOW, 10);
			data |= digitalRead(dat) << i;
			wait(clk, HIGH, 10);
		}
		// eat parity bit, ignore it.
		wait(clk, LOW, 10);
		wait(clk, HIGH, 10);
		// eat stop bit
		wait(clk, LOW, 10);
		wait(clk, HIGH, 10);
		return data;
	}

	void write(uint8_t data) {
		uint8_t parity = 1;
		gohi(dat);
		gohi(clk);
		delayMicroseconds(300);
		golo(clk);
		delayMicroseconds(300);
		golo(dat);
		delayMicroseconds(150);
		gohi(clk);	// start bit
		/* wait for device to take control of clock */
		wait(clk, LOW, 10);	// this loop intentionally left blank
		// clear to send data
		
		for (uint8_t i=0; i < 8; i++)
		{
			digitalWrite(dat, data & 0x01);
			// wait for clock
			wait(clk, HIGH, 10);
			wait(clk, LOW, 10);
			parity ^= (data & 0x01);
			data >>= 1;
		}
		// parity bit
		digitalWrite(dat, parity);
		// clock cycle - like ack.
		wait(clk, HIGH, 10);
		wait(clk, LOW, 10);
		// stop bit
		gohi(dat);
		wait(clk, HIGH, 10);
		wait(clk, LOW, 10);
		// mode switch
		wait(clk, HIGH, 10);
		wait(dat, HIGH, 10);
		// hold up incoming data
		golo(clk);	// hold incoming data
	}

protected:
	uint8_t clk, dat;
	Queue queue;
	uint8_t bitcount;
	uint8_t incoming;
	uint8_t is_init;
	uint32_t prev_ms;

	void gohi(uint8_t pin)
	{
		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);
	}

	void golo(uint8_t pin)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
	
	void wait(uint8_t pin, uint8_t value, uint16_t d) {
		uint32_t m = millis();
		while (digitalRead(pin) != value && millis() - m < d);
	}
};

class PS2Keyboard: public PS2 {
public:
	PS2Keyboard(uint8_t size, uint8_t clk, uint8_t dat): PS2(size, clk, dat) {}
	void begin() override {
		write(0xF0); // Установить Scan Code
		if (read() != 0xFA) return;
		write(0x01); // ..1
		if (read() != 0xFA) return;
		write(0xF8); // Режим make/release
		if (read() != 0xFA) return;
		write(0xED); // ..1
		if (read() != 0xFA) return;
		write(0x04); // Режим make/release
		if (read() != 0xFA) return;
		is_init = 1;
		pinMode(clk, INPUT_PULLUP);
		pinMode(dat, INPUT_PULLUP);
	}
};

class PS2Mouse: public PS2 {
public:
	PS2Mouse(uint8_t size, uint8_t clk, uint8_t dat): PS2(size, clk, dat) {}
	void begin() override {
		write(0xFF); // резет
		if (read() != 0xFA) return;
		write(0xF6); // дефолты
		if (read() != 0xFA) return;
		write(0xF4); // отчеты
		if (read() != 0xFA) return;
		is_init = 1;
		pinMode(clk, INPUT_PULLUP);
		pinMode(dat, INPUT_PULLUP);
	}
};
