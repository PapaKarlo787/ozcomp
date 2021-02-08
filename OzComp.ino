#include <Arduino.h>
#include <sd_raw.h>
#include <PS2Keyboard.h>
#include <MyScreen.h>
#include "arch/arch.h"

void setup() {
  if (!sd_raw_init()) while(true);
  lcd.begin();
  keyboard.begin(8, 3);
  randomSeed(analogRead(0));
  lcd.setCursor(0, 0);
  while(true){
    uint8_t x = read();
    void (*f) (void) = pgm_read_word(&comms[x]);
    (*f)();
  }
}

void loop() {
  
}
