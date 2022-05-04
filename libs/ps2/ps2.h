/*
 * ps2.h - a library to interface with ps2 devices. See comments in
 * ps2.cpp.
 * Written by Chris J. Kiick, January 2008.
 * Release into public domain.
 */

#ifndef ps2_h
#define ps2_h

#include "Arduino.h"

class PS2
{
	public:
		PS2(uint8_t clk, uint8_t data);
		void write(uint8_t data);
		uint8_t read();
		bool detected();
	private:
		void golo(uint8_t pin);
		void gohi(uint8_t pin);
		uint8_t _ps2clk;
		uint8_t _ps2data;
		uint8_t was_det;
};

#endif /* ps2_h */

