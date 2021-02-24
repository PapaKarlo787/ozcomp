#include <Arduino.h>
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include <GyverTimers.h>
#include "arch/arch.h"

void setup() {
  pinMode(17, OUTPUT);
  while (!sd_raw_init()) {};
  lcd.begin();
  Timer0.setPeriod(10000);
  Timer0.enableISR();
  keyboard.begin(8, 3);
  randomSeed(analogRead(0));
  lcd.setCursor(0, 0);
}

void loop() {
  void (*f) (void) = pgm_read_word(&comms[read_()]);
  (*f)();
}
