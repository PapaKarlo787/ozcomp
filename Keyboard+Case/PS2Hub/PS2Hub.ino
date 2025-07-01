#include <PS2Keyboard.h>
#include <ps2dev.h>
#include <NecDecoder.h>

NecDecoder ir;
PS2Keyboard kbd(64, 6, 7);
PS2Mouse mice(128, 4, 5);
Queue joystick(64);
Queue IR(64);
PS2dev host(8, 9);
uint8_t host_sends = false;

ISR(PCINT2_vect) { // keyboard/mouse
  kbd.readBit();
  mice.readBit();
}

ISR(PCINT1_vect) { // Joystick
  joystick.enqueue(PINC & 31);
}

ISR(PCINT0_vect) { // Host
  host_sends = true;
}

void tick() {
  ir.tick();
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, tick, FALLING);
  DDRC = 0x00;
  PORTC = 0xFF;
  kbd.begin();
  mice.begin();
  cli();
  PCIFR |= (1 << PCIE2) | (1 << PCIE1) | (1 << PCIE0);
  PCICR |= (1 << PCIE2) | (1 << PCIE1) | (1 << PCIE0);
  PCMSK2 |= 64 + 16;
  PCMSK1 |= 31;
  PCMSK0 |= 1;
  sei();
  delay(500);
  Serial.println("Hello");
}

void loop() {
  uint8_t x;
  if (host_sends) {
    PCMSK2 &= ~1;
    host_sends = false;
    host.read(&x);
    Serial.println(x);
    PCMSK2 |= 1;
  }
  x = kbd.take();
  if (x) {
    Serial.print("K ");
    Serial.println(x);
  }

  if (ir.available()) {
    IR.enqueue(ir.readCommand());
    IR.dequeue(&x);
    Serial.println(x, HEX);
  }
  
  if (!joystick.dequeue(&x)) {
    Serial.print("J ");
    Serial.println(x);
  }
  
  x = mice.take();
  if (x) {
    Serial.print("M ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(mice.take());
    Serial.print(" ");
    Serial.println(mice.take());
  }
}
