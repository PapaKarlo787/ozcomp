#include "arch.cpp"


void setup() {
  Serial.begin(9600);
  OzArch a;
  while(true) a.exec();
}

void loop() {
  
}
