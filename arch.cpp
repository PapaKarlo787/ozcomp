#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <Arduino.h>
#include <MyScreen.h>


class OzArch
{
public:
	OzArch() {
		if (!sd_raw_init()) while(true);
		keyboard.begin(8, 3);
		lcd.begin();
		randomSeed(analogRead(0));
    lcd.setCursor(0, 0);
	}
  
	void exec() {
		uint8_t x = read();
		(this->*comms[x])();
	}

private:
	uint8_t flags;
	uint32_t ip = 0;
	uint32_t sp = 1073741824;
	double R[16];
	uint8_t r1, r2;
	PS2Keyboard keyboard;
	PCD8544 lcd;
  
	void (OzArch::*comms[43]) (void) = {
		&OzArch::add_rr, &OzArch::add_rc, &OzArch::sub_rr, &OzArch::sub_rc, &OzArch::mul_rr, &OzArch::mul_rc,
		&OzArch::div_rr, &OzArch::div_rc, &OzArch::mov_rm, &OzArch::mov_mr, &OzArch::mov_rr, &OzArch::mov_rc,
		&OzArch::mov_rmor, &OzArch::mov_morr, &OzArch::and_rr, &OzArch::and_rc, &OzArch::or_rr, &OzArch::or_rc,
		&OzArch::xor_rr, &OzArch::xor_rc, &OzArch::cmp_rr, &OzArch::cmp_rc, &OzArch::jmp, &OzArch::jmp_c,
		&OzArch::lp, &OzArch::push, &OzArch::pop, &OzArch::mod_rr, &OzArch::mod_rc, &OzArch::print_num,
		&OzArch::print_line, &OzArch::del_r, &OzArch::del_c, &OzArch::snd_r, &OzArch::snd_c, &OzArch::to_int,
		&OzArch::get_key, &OzArch::set_cursor, &OzArch::set_cursor_r, &OzArch::draw_screen, &OzArch::call, &OzArch::ret,
		&OzArch::rnd
	};
	
	void readRegisters() {
		uint8_t i = read();
		r1 = i & 15;
		r2 = (i & 240) / 16;
	}
  
	uint32_t readNum(uint8_t n) {
		uint32_t x = 0;
		for (uint8_t i = 0; i < n; i++)
			x = x * 256 + read();
		return x;
	}
	
	void setFlags(double x) {
		flags = 0;
		flags += x > 0 ? 1 : 0;
		flags *= 2;
		flags += x < 0 ? 1 : 0;
		flags *= 2;
		flags += x == 0 ? 1 : 0;
	}

	uint8_t read(){
		uint8_t temp[1];
		sd_raw_read(ip, temp, 1);
		ip++;
		return temp[0];
	}

	void write(uint8_t* data, uint32_t n){
		sd_raw_write(ip, data, n);
		sd_raw_sync();
	}

	void write(uint32_t poi, uint8_t* data, uint32_t n){
		sd_raw_write(poi, data, n);
		sd_raw_sync();
	}

	void add_rr() {
		readRegisters();
		R[r1] += R[r2];
	}

	void add_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = R[r1] + *(double*)&x;
	}
	
	void sub_rr() {
		readRegisters();
		R[r1] -= R[r2];
	}

	void sub_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] -= *(double*)&x;
	}
		
	void mul_rr() {
		readRegisters();
		R[r1] *= R[r2];
	}

	void mul_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] *= *(double*)&x;
	}

	void div_rr() {
		readRegisters();
		R[r1] /= R[r2];
	}
	
	void div_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] /= *(double*)&x;
	}
	
	void mov_rm() {
		readRegisters();
		uint32_t temp = ip;
		ip = readNum(4);
		uint32_t x = readNum(4);
		R[r1] = *(double*)&x;
		ip = temp + 4;
	}

	void mov_mr() {
		readRegisters();
		write(readNum(4), (uint8_t*)&(R[r1]), 4);
	}

	void mov_rr() {
		readRegisters();
		R[r1] = R[r2];
	}

	void mov_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = *(double*)&x;
	}

//r1 - destination, r2 - index
	void mov_rmor() {
		readRegisters();
		uint32_t temp = ip;
		ip = readNum(4)+((uint32_t)R[r2]);
		uint32_t x = readNum(4);
		R[r1] = *(double*)&x;
		ip = temp + 4;
	}

	void mov_morr() {
		readRegisters();
		write(readNum(4)+((uint32_t)R[r2]), (uint8_t*)&(R[r1]), 4);
	}
	
	void jmp() {
		ip = (uint32_t)readNum(4);
	}

//zero, less, greater
//если есть n то перевернуть все биты
	void jmp_c() {
		uint8_t f = (uint8_t)readNum(1);
		if((f & flags) != 0)
			jmp();
		else
			ip += 4;
	}

	void and_rr() {
		readRegisters();
		R[r1] = (uint32_t)R[r1] & (uint32_t)R[r2];
	}
	
	void and_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = (uint32_t)R[r1] & (uint32_t)(*(double*)&x);
	}
	
	void or_rr() {
		readRegisters();
		R[r1] = (uint32_t)R[r1] | (uint32_t)R[r2];
	}
	
	void or_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = (uint32_t)R[r1] | (uint32_t)(*(double*)&x);
	}
	
	void xor_rr() {
		readRegisters();
		R[r1] = (uint32_t)R[r1] ^ (uint32_t)R[r2];
	}
	
	void xor_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = (uint32_t)R[r1] ^ (uint32_t)(*(double*)&x);
	}

	void cmp_rr() {
		readRegisters();
		setFlags(R[r1] - R[r2]);
	}

	void cmp_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		setFlags(R[r1] - *(double*)&x);
	}
  
	void get_key() {
		R[15] = keyboard.available() ? keyboard.read() : 0;
	}
	
	void print_line() {
		uint32_t temp = ip;
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
		write(sp -= 4, (uint8_t*)&(R[r1]), 4);
	}

	void pop() {
		readRegisters();
		uint32_t temp = ip;
		ip = sp;
		uint32_t x = readNum(4);
		R[r1] = *(double*)&x;
		ip = temp;
		sp += 4;
	}

	void to_int() {
		readRegisters();
		R[r1] = (uint32_t)R[r1];
	}

	void mod_rr() {
		readRegisters();
		R[r1] = (uint32_t)R[r1] % (uint32_t)R[r2];
	}

	void mod_rc() {
		readRegisters();
		uint32_t x = readNum(4);
		R[r1] = (uint32_t)R[r1] % (uint32_t)(*(double*)&x);
	}

	void del_r() {
		readRegisters();
		delay((uint32_t)R[r1]);
	}

	void del_c() {
		uint32_t x = readNum(4);
		delay((uint32_t)*(double*)&x);
	}

	void snd_r() {
		readRegisters();
		lcd.send(HIGH, (uint8_t)R[r1]);
	}
  
	void snd_c() {
		lcd.send(HIGH, (uint8_t)readNum(1));
	}

	void set_cursor() {
		byte x = (byte)readNum(1);
		byte y = (byte)readNum(1);
		lcd.setCursor(x, y);
	}

	void set_cursor_r() {
		readRegisters();
		lcd.setCursor(R[r1], R[r2]);
	}

	void draw_screen() {
		uint32_t temp = ip;
		ip = readNum(4);
		for (int i = 0; i < 504; i++)
			lcd.send(HIGH, (uint8_t)readNum(1));
		ip = temp + 4;
	}

	void call() {
		uint32_t temp = readNum(4);
		write(sp-=4, (uint8_t*)&(ip), 4);
		for (byte i = 0; i < 16; i++)
			write(sp-=4, (uint8_t*)&(R[i]), 4);
		ip = temp;
	}
	
	void ret() {
		ip = sp;
		for (byte i = 15; i > -1; i--) {
			sp += 4;
			uint32_t x = readNum(4);
			R[i] = *(double*)&x;
		}
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
