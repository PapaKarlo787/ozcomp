#include "Arduino.h"
#include "PS2Keyboard.h"

#define BUFFER_SIZE 20
#define ps2clk 3
#define dataPin 8
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;

void ps2interrupt(void) {
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(dataPin);
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

uint8_t PS2Keyboard::take() {
	if (tail == head) return 0;
	uint8_t res = buffer[tail];
	if (++tail >= BUFFER_SIZE) tail = 0;
	return res;
}

void PS2Keyboard::begin(){
	pinMode(ps2clk, INPUT_PULLUP);
	pinMode(dataPin, INPUT_PULLUP);
	head = 0;
	tail = 0;
}

void PS2Keyboard::write(uint8_t data)
{
	uint8_t parity = 1;
	gohi(dataPin);
	gohi(ps2clk);
	delayMicroseconds(300);
	golo(ps2clk);
	delayMicroseconds(300);
	golo(dataPin);
	delayMicroseconds(150);
	gohi(ps2clk);	// start bit
	/* wait for device to take control of clock */
	while (digitalRead(ps2clk) == HIGH) ;	// this loop intentionally left blank
	// clear to send data
	
	for (uint8_t i=0; i < 8; i++)
	{
		digitalWrite(dataPin, data & 0x01);
		// wait for clock
		while (digitalRead(ps2clk) == LOW) ;
		while (digitalRead(ps2clk) == HIGH) ;
		parity ^= (data & 0x01);
		data >>= 1;
	}
	// parity bit
	digitalWrite(dataPin, parity);
	// clock cycle - like ack.
	while (digitalRead(ps2clk) == LOW) ;
	while (digitalRead(ps2clk) == HIGH) ;
	// stop bit
	gohi(dataPin);
	while (digitalRead(ps2clk) == LOW) ;
	while (digitalRead(ps2clk) == HIGH) ;
	// mode switch
	while ((digitalRead(ps2clk) == LOW) || (digitalRead(dataPin) == LOW)) ;
	// hold up incoming data
	golo(ps2clk);	// hold incoming data
}

/*
 * read a byte of data from the ps2 device.  Ignores parity.
 */
uint8_t PS2Keyboard::read() {
	uint8_t data = 0x00;

	// start clock
	gohi(ps2clk);
	gohi(dataPin);
	while (digitalRead(ps2clk) == HIGH);

	while (digitalRead(ps2clk) == LOW);	// eat start bit
	for (uint8_t i = 0; i < 8; i++) {
		while (digitalRead(ps2clk) == HIGH) ;
		data |= digitalRead(dataPin) << i;
		while (digitalRead(ps2clk) == LOW) ;
	}
	// eat parity bit, ignore it.
	while (digitalRead(ps2clk) == HIGH);
	while (digitalRead(ps2clk) == LOW);
	// eat stop bit
	while (digitalRead(ps2clk) == HIGH);
	while (digitalRead(ps2clk) == LOW);
	return data;
}

PS2Keyboard::PS2Keyboard() { }
