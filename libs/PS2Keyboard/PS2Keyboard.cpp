#include "Arduino.h" 
#include "PS2Keyboard.h"

#define BUFFER_SIZE 20
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;

void ps2interrupt(void)
{
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;

	uint8_t val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250)
		bitcount = incoming = 0;
	prev_ms = now_ms;

	if (bitcount < 9)
		incoming |= (val << bitcount-1);

	if (++bitcount == 11) {
		uint8_t i = head < BUFFER_SIZE-1 ? head + 1 : 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = incoming = 0;
	}
}

bool PS2Keyboard::available() {
	return tail != head;
}

int PS2Keyboard::read() {
	if (tail == head) return 0;
	tail++;
	if (tail >= BUFFER_SIZE) tail = 0;
	return buffer[tail];
}

PS2Keyboard::PS2Keyboard() {}

void PS2Keyboard::begin(uint8_t dataPin, uint8_t irq_pin) {
  DataPin = dataPin;
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(dataPin, INPUT_PULLUP);
  head = 0;
  tail = 0;
  attachInterrupt(1, ps2interrupt, FALLING);
}
