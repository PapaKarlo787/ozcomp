#include "Arduino.h" 
#include "ps2.h"
#include "PS2Keyboard.h"

#define BUFFER_SIZE 20
#define ps2clk 3
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;

void ps2interrupt(void) {
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
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
	bitcount++;
	if (bitcount == 11) {
		if (incoming == 1) {
			
		}
		buffer[head] = incoming;
		head = (head+1) % BUFFER_SIZE;
		bitcount = 0;
		incoming = 0;
	}
}

bool PS2Keyboard::available() {
	return tail != head;
}

uint8_t PS2Keyboard::read() {
	if (tail == head) return 0;
	uint8_t res = buffer[tail];
	if (++tail >= BUFFER_SIZE) tail = 0;
	return res;
}

void PS2Keyboard::begin(uint8_t dataPin){
  PS2 k(3, dataPin);
  k.write(0xf0);
  k.read();
  k.write(3);
  DataPin = dataPin;
  pinMode(ps2clk, INPUT_PULLUP);
  pinMode(dataPin, INPUT_PULLUP);
  head = 0;
  tail = 0;
  attachInterrupt(1, ps2interrupt, FALLING);
}

PS2Keyboard::PS2Keyboard() { }
