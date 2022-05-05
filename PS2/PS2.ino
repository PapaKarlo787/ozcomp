#include <ps2dev.h>
#include <ps2.h>
#include <IRremote.h>
#include "lpstack.h"

PS2 kbd(3, 4);
PS2 mice(5, 6);
PS2dev ps2out(A5, A4);
Lpstack kst(33);
Lpstack irst(33);
Lpstack jst(33);
Lpstack mst(100);
IRrecv irrecv(A3);

Lpstack *const datas[4] PROGMEM = {&kst, &mst, &jst, &irst};

void setup() {
  uint8_t pins[] = {7, 13+128, A0, A1+128, A2, 2+128};
  irrecv.enableIRIn();
  Serial.begin(9600);
  for (uint8_t i : pins) {
    pinMode(i&127, OUTPUT);
    digitalWrite(i&127, i>>7);
  }
  for (uint8_t i = 8; i < 13; i++){
    pinMode(i, INPUT);
    digitalWrite(i, 1);
  }
  
  uint8_t data[] = {0xE8, 3, 0xE6, 0xF3, 40, 0xF4};
  for (uint8_t i : data) {
    mice.write(i);
    mice.read();
  }
  
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
  uint8_t res = 0;
  ps2out.read(&res);
  Lpstack *dev = pgm_read_word(&datas[res]);
  res = (*dev).pop();
  while(ps2out.write(res) < 0) ;
}

void readIR(decode_results results) {
  irst.push(results.value);
  irrecv.resume();
}

void checkJoystick(){
  uint8_t res = PINB & 31;
  if (res^31) jst.push(res);
}

void loop() {
  decode_results results;
  if (kbd.detected()) readKbd();
  if (mice.detected()) readMise();
  if (ps2out.available()) readOut();
  if (irrecv.decode(&results)) readIR(results);
  checkJoystick();
}
