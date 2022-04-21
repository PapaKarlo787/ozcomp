#include <ps2.h>
#include "lpstack.h"

PS2 kbd(3, 4);
PS2 mice(5, 6);
PS2 ps2out(A5, A4);
Lpstack mst(33);
Lpstack kst(100);

Lpstack* const datas[2] PROGMEM = {&kst, &mst};

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  kbd.write(0xF0);
  kbd.read();
  kbd.write(0x01);
  kbd.read();
  kbd.write(0xF8);
  kbd.read();
}

void readKbd() {
  kst.push(kbd.read());
}

void readMise() {
  mst.push(mice.read());
  mst.push(mice.read());
  mst.push(mice.read());
}

void readOut() {
  uint8_t res = ps2out.read();
  Lpstack dev = pgm_read_word(datas[res]);
  ps2out.write(dev.pop());
}

void loop() {
  if (!digitalRead(3)) readKbd();
  if (!digitalRead(5)) readMise();
  if (!digitalRead(A5)) readOut();
}
