uint8_t flags;
uint32_t ip = 0;
uint32_t sp = 1073741824;
float R[16];
uint8_t r1, r2;
uint8_t color = 255;
PS2Keyboard keyboard;
PCD8544 lcd;
uint8_t screen_buffer[504];

uint8_t read(){
  uint8_t temp[1];
  sd_raw_read(ip, temp, 1);
  ip++;
  return temp[0];
}
void readRegisters() {
  uint8_t i = read();
  r1 = i & 15;
  r2 = (i & 240) / 16;
}

uint32_t readNum(uint8_t n) {
  uint32_t x = 0;
  for (uint8_t i = 0; i < n; i++)
    x += read() << (8 * i);
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

void write(uint8_t* data, uint32_t n){
  sd_raw_write(ip, data, n);
  sd_raw_sync();
}

void write(uint32_t poi, uint8_t* data, uint32_t n){
  sd_raw_write(poi, data, n);
  sd_raw_sync();
}


#include "ariphmetics.h"
#include "moves.h"
#include "screen_kbd.h"
#include "logics.h"
#include "jumps_and_stack.h"
#include "geometry.h"
#include "bmp.h"


void to_int() {
  readRegisters();
  R[r1] = (uint32_t)R[r1];
}

void del_r() {
  readRegisters();
  delay((uint32_t)R[r1]);
}

void del_c() {
  uint32_t x = readNum(4);
  delay((uint32_t)*(float*)&x);
}

void rnd(){
  R[13] = random(1024);
}

#include "instructions.h"
