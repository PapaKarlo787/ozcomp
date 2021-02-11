#include "Arduino.h"
#include "sd_raw.h"
#include "PS2Keyboard.h"
#include "MyScreen.h"
#include <iostream>


class OzArch {
public:
	OzArch() {
		if (!sd_raw_init()) while(true);
		lcd.begin();
		keyboard.begin(8, 3);
		randomSeed(analogRead(0));
		lcd.setCursor(0, 0);
		for (int i = 0; i < 504; i++)
			screen_buffer[i] = 0;
	}
  
	void exec() {
		char x = read();
		(this->*comms[x])();
	}

private:
	char flags;
	unsigned int ip = 0;
	unsigned int sp = 1073741824;
	float R[16];
	char r1, r2;
	unsigned char color = 255;
	PS2Keyboard keyboard;
	PCD8544 lcd;
	unsigned char screen_buffer[504];

#include "arch/instructions.h"
	
	void readRegisters() {
		unsigned char i = read();
		r1 = i & 15;
		r2 = (i & 240) / 16;
	}
  
	int readNum(char n) {
		int x = 0;
		for (char i = 0; i < n; i++)
			x = x * 256 + read();
		return x;
	}
	
	void setFlags(float x) {
		flags &= 248;
		flags += x > 0 ? 1 : 0;
		flags *= 2;
		flags += x < 0 ? 1 : 0;
		flags *= 2;
		flags += x == 0 ? 1 : 0;
	}

	unsigned char read(){
		unsigned char temp[1];
		sd_raw_read(ip, temp, 1);
		ip++;
		return temp[0];
	}

	void write(unsigned char* data, unsigned int n){
		sd_raw_write(ip, data, n);
		sd_raw_sync();
	}

	void write(unsigned int poi, unsigned char* data, unsigned int n){
		sd_raw_write(poi, data, n);
		sd_raw_sync();
	}


#include "arch/ariphmetics.h"
#include "arch/moves.h"
#include "arch/screen_kbd.h"
#include "arch/logics.h"
#include "arch/jumps_and_stack.h"
#include "arch/geometry.h"
#include "arch/bmp.h"


	void to_int() {
		readRegisters();
		setFlags(R[r1] = (unsigned int)R[r1]);
	}

	void del_r() {
		readRegisters();
		delay((unsigned int)R[r1]);
	}

	void del_c() {
		unsigned int x = readNum(4);
		delay((unsigned int)*(float*)&x);
	}

	void rnd(){
		R[13] = random(1024);
	}
};
