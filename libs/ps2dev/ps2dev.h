/*
 * ps2dev.h - a library to interface with ps2 hosts. See comments in
 * ps2.cpp.
 * Written by Chris J. Kiick, January 2008.
 * modified by Gene E. Scogin, August 2008.
 * Release into public domain.
 */

#ifndef ps2dev_h
#define ps2dev_h
#include "Arduino.h"

class PS2dev
{
	public:
		PS2dev(uint8_t clk, uint8_t data);
		void write(uint16_t data);
		uint8_t read();
	private:
		void reset();
		void golo(int pin);
		void gohi(int pin);
		uint8_t _ps2clk;
		uint8_t	_ps2data;
};
#endif
