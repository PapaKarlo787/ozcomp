#include <Arduino.h>
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include <iarduino_RTC.h>
#include <ps2.h>
#include "arch/arch.h"
#define V2


void setup() {
  PS2 kbd(3, 8);
  kbd.write(0xF0); //Установить Scan Code
  kbd.read();
  kbd.write(0x01); //...3
  kbd.read();
  kbd.write(0xF8); // Режим make/release
  kbd.read();
  while (!sd_raw_init()) {};
  lcd.begin();
  Serial.begin(115200);
  keyboard.begin();
  time.begin();
  randomSeed(analogRead(0));
  sd_raw_info info;
  sd_raw_get_info(&info);
  sp = info.capacity;
}

void loop() {
  void (*f) (void) = pgm_read_word(&comms[read_()]);
  (*f)();
  fill_mbuffer();
}
