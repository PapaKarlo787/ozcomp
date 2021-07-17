#include <Arduino.h>
#include "arch/gloabal_simle_vars.h"
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include "arch/arch.h"

void setup() {
  pinMode(17, OUTPUT);
  while (!sd_raw_init()) {};
  lcd.begin();
  keyboard.begin(8, 3);
  randomSeed(analogRead(0));
}

void loop() {
  void (*f) (void) = pgm_read_word(&comms[read_()]);
  (*f)();
  check_play();
}
