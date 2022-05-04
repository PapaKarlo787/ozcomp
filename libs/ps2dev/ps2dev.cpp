#include "ps2dev.h"

#define CLKFULL 40
#define CLKHALF 20

#define BYTEWAIT 1000
#define TIMEOUT 30

/*
 * according to some code I saw, these functions will
 * correctly set the clock and data pins for
 * various conditions.  It's done this way so you don't need
 * pullup resistors.
 */
void PS2dev::gohi(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2dev::golo(int pin)
{
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

void PS2dev::reset() {
	delayMicroseconds(CLKHALF);
	golo(_ps2clk);
	delayMicroseconds(CLKFULL);
	gohi(_ps2clk);
	delayMicroseconds(CLKHALF);
}

PS2dev::PS2dev(uint8_t clk, uint8_t data) {
	_ps2clk = clk;
	_ps2data = data;
}

void PS2dev::write(uint16_t data) {
  delayMicroseconds(BYTEWAIT);
  uint8_t parity = 1;
  data = data << 1 | 1024;

  for (uint8_t i=0; i < 11; i++) {
	data & 0x01 ? gohi(_ps2data) : golo(_ps2data);
	reset();
	parity ^= (data & 0x01);
	data >>= 1;
  }

  delayMicroseconds(BYTEWAIT);
}

uint8_t PS2dev::read()
{
  uint16_t data = 0;
  uint8_t parity = 1;

  //wait for data line to go low and clock line to go high (or timeout)
  uint32_t waiting_since = millis();
  while((digitalRead(_ps2data) != LOW) || (digitalRead(_ps2clk) != HIGH)) {
    if((millis() - waiting_since) > TIMEOUT) return 0;
  }

  for (uint8_t i = 0; i < 11; i++) {
	data |= digitalRead(_ps2data) << i;
    reset();
  }

  delayMicroseconds(CLKHALF);
  golo(_ps2data);
  golo(_ps2clk);
  delayMicroseconds(CLKFULL);
  gohi(_ps2clk);
  delayMicroseconds(CLKHALF);
  gohi(_ps2data);
  return parity == 3 || parity == 0 ? (data >> 1) & 0xFF : 0;
}
