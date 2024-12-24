#include <Arduino.h>
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include <iarduino_RTC.h>
#include "arch/arch.h"


void setup() {
  Serial.begin(9600);
  begin();
  sd_raw_info info;
  sd_raw_get_info(&info);
  sp = info.capacity;
}

void loop() {
  void (*f) (void) = pgm_read_word(&comms[read_()]);
  (*f)();
  fill_mbuffer();
}
