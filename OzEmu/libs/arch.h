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
	int ip = 0;
	int sp = 1073741824;
	float R[16];
	char r1, r2;
	PS2Keyboard keyboard;
	PCD8544 lcd;
	unsigned char screen_buffer[504];

	void (OzArch::*comms[256]) (void) = {
		&OzArch::add_rr, 
		&OzArch::add_rc, 
		&OzArch::sub_rr, 
		&OzArch::sub_rc, 
		&OzArch::mul_rr, 
		&OzArch::mul_rc,
		&OzArch::div_rr, 
		&OzArch::div_rc, 
		&OzArch::mov_rm, 
		&OzArch::mov_mr, 
		&OzArch::mov_rr, 
		&OzArch::mov_rc,
		&OzArch::mov_rmor, 
		&OzArch::mov_morr, 
		&OzArch::and_rr, 
		&OzArch::and_rc, 
		&OzArch::or_rr, 
		&OzArch::or_rc,
		&OzArch::xor_rr, 
		&OzArch::xor_rc, 
		&OzArch::cmp_rr, 
		&OzArch::cmp_rc, 
		&OzArch::jmp,
		&OzArch::jmp_c,
		&OzArch::lp, 
		&OzArch::push, 
		&OzArch::pop, 
		&OzArch::mod_rr, 
		&OzArch::mod_rc, 
		&OzArch::print_num,
		&OzArch::print_line, 
		&OzArch::del_r,
		&OzArch::del_c,
		&OzArch::snd_r,
		&OzArch::snd_c, 
		&OzArch::to_int,
		&OzArch::get_key, 
		&OzArch::set_cursor, 
		&OzArch::set_cursor_r, 
		&OzArch::draw_screen,
		&OzArch::call, 
		&OzArch::ret,
		&OzArch::rnd,
		&OzArch::print_int,
		&OzArch::movb_rm,
		&OzArch::movb_mr,
		&OzArch::movb_rmor,
		&OzArch::movb_morr,
		&OzArch::pow_rr,
		&OzArch::pow_rc, 
		&OzArch::point_rr,
		&OzArch::point_cc,
		&OzArch::circle_r,
		&OzArch::circle_c,
		&OzArch::line_r,
		&OzArch::line_c,
		&OzArch::clear_screen
	};
	
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
		flags = 0;
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

	void draw_screen() {
		lcd.setCursor(0, 0);
		for(unsigned int i = 0; i < 504; i++)
			lcd.send(HIGH, screen_buffer[i]);
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
	
#include "new_features.h"
};
