#include <Arduino.h>
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include "arch/arch.h"

void setup() {
  pinMode(17, OUTPUT);
  while (!sd_raw_init()) {};
  lcd.begin();
  keyboard.begin();
  randomSeed(analogRead(0));
  sp = sd_raw_get_size();
}

void loop() {
  void (*f) (void) = pgm_read_word(&comms[read_()]);
  (*f)();
}
