#include <iostream>
#include "Arduino.h"
#include "sd_raw.h"
#include "PS2Keyboard.h"
#include "MyScreen.h"


class OzArch {
public:
	OzArch() {
		if (!sd_raw_init()) while(true);
		lcd.begin();
		keyboard.begin(8, 3);
		randomSeed(analogRead(0));
		lcd.setCursor(0, 0);
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
  
	void (OzArch::*comms[43]) (void) = {
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
		&OzArch::rnd
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

	void write(unsigned int poi, unsigned char* data, unsigned int n){
		sd_raw_write(poi, data, n);
		sd_raw_sync();
	}

	void add_rr() {
		readRegisters();
		R[r1] += R[r2];
	}

	void add_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = R[r1] + *(float*)&x;
	}
	
	void sub_rr() {
		readRegisters();
		R[r1] -= R[r2];
	}

	void sub_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] -= *(float*)&x;
	}
		
	void mul_rr() {
		readRegisters();
		R[r1] *= R[r2];
	}

	void mul_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] *= *(float*)&x;
	}

	void div_rr() {
		readRegisters();
		R[r1] /= R[r2];
	}
	
	void div_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] /= *(float*)&x;
	}
	
	void mov_rm() {
		readRegisters();
		unsigned int temp = ip;
		ip = readNum(4);
		unsigned int x = readNum(4);
		R[r1] = *(float*)&x;
		ip = temp + 4;
	}

	void mov_mr() {
		readRegisters();
		write(readNum(4), (unsigned char*)&(R[r1]), 4);
	}

	void mov_rr() {
		readRegisters();
		R[r1] = R[r2];
	}

	void mov_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = *(float*)&x;
	}

//r1 - destination, r2 - index
	void mov_rmor() {
		readRegisters();
		unsigned int temp = ip;
		ip = readNum(4)+((unsigned int)R[r2]);
		unsigned int x = readNum(4);
		R[r1] = *(float*)&x;
		ip = temp + 4;
	}

	void mov_morr() {
		readRegisters();
		write(readNum(4)+((unsigned int)R[r2]), (unsigned char*)&(R[r1]), 4);
	}
	
	void jmp() {
		ip = (unsigned int)readNum(4);
	}

//zero, less, greater
//если есть n то перевернуть все биты
	void jmp_c() {
		char f = (char)readNum(1);
		
		std::cout<<(int)f<<std::endl;
		if((f & flags) != 0)
			jmp();
		else
			ip += 4;
	}

	void and_rr() {
		readRegisters();
		R[r1] = (unsigned int)R[r1] & (unsigned int)R[r2];
	}
	
	void and_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = (unsigned int)R[r1] & (unsigned int)(*(float*)&x);
	}
	
	void or_rr() {
		readRegisters();
		R[r1] = (unsigned int)R[r1] | (unsigned int)R[r2];
	}
	
	void or_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = (unsigned int)R[r1] | (unsigned int)(*(float*)&x);
	}
	
	void xor_rr() {
		readRegisters();
		R[r1] = (unsigned int)R[r1] ^ (unsigned int)R[r2];
	}
	
	void xor_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = (unsigned int)R[r1] ^ (unsigned int)(*(float*)&x);
	}

	void cmp_rr() {
		readRegisters();
		setFlags(R[r1] - R[r2]);
	}

	void cmp_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		setFlags(R[r1] - *(float*)&x);
	}
  
	void get_key() {
		R[15] = keyboard.available() ? keyboard.read() : 0;
	}
	
	void print_line() {
		unsigned int temp = ip;
		ip = readNum(4);
		while (char c = read())
			lcd.write(c);
		ip = temp + 4;
	}
	
	void print_num() {
		readRegisters();
		lcd.print(R[r1]);
	}

	void push() {
		readRegisters();
		write(sp -= 4, (unsigned char*)&(R[r1]), 4);
	}

	void pop() {
		readRegisters();
		unsigned int temp = ip;
		ip = sp;
		unsigned int x = readNum(4);
		R[r1] = *(float*)&x;
		ip = temp;
		sp += 4;
	}

	void to_int() {
		readRegisters();
		R[r1] = (unsigned int)R[r1];
	}

	void mod_rr() {
		readRegisters();
		R[r1] = (unsigned int)R[r1] % (unsigned int)R[r2];
	}

	void mod_rc() {
		readRegisters();
		unsigned int x = readNum(4);
		R[r1] = (unsigned int)R[r1] % (unsigned int)(*(float*)&x);
	}

	void del_r() {
		readRegisters();
		delay((unsigned int)R[r1]);
	}

	void del_c() {
		unsigned int x = readNum(4);
		delay((unsigned int)*(float*)&x);
	}

	void snd_r() {
		readRegisters();
		lcd.send(HIGH, (char)R[r1]);
	}
  
	void snd_c() {
		lcd.send(HIGH, (char)readNum(1));
	}

	void set_cursor() {
		char x = (char)readNum(1);
		char y = (char)readNum(1);
		lcd.setCursor(x, y);
	}

	void set_cursor_r() {
		readRegisters();
		lcd.setCursor(R[r1], R[r2]);
	}

	void draw_screen() {
		unsigned int temp = ip;
		ip = readNum(4);
		for (unsigned int i = 0; i < 504; i++)
			lcd.send(HIGH, (char)readNum(1));
		ip = temp + 4;
	}

	void call() {
		unsigned int old_ip = ip + 4;
		ip = readNum(4);
		for (char i = 0; i < 4; i++) { 
			unsigned char t[1];
			t[0] = old_ip % 256;
			old_ip /= 256;
			write(sp-=1, t, 1);
		}
	}
	
	void ret() {
		ip = sp;
		ip = readNum(4);
		sp += 4;
	}

	void lp() {
		ip = R[14]-- > 0 ? readNum(4) : ip + 4;
	}
  
	void rnd(){
		R[13] = random(1024);
	}
};
