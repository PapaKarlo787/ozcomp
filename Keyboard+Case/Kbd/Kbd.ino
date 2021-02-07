#include <Keyboard.h>
static unsigned long timer = 0;
char key;
char pkey;
const int P[] = {8,9,15,14,16,10,A0,A1,A2,A3};
const int M[] = {2,3,4,5,6,7};
const char k6x6 [2][6][6] = {
  {
    {'z', 'c', 'b', 'm', 'p', '6'},
    {'a', 'd', 'g', 'j', 'l', '5'},
    {'q', 'e', 't', 'u', '0', '4'},
    {'w', 'r', 'y', 'i', '9', '3'},
    {'s', 'f', 'h', 'k', '8', '2'},
    {'x', 'v', 'n', 'o', '7', '1'},
  },
  {
    {'Z', 'C', 'B', 'M', 'P', '^'},
    {'A', 'D', 'G', 'J', 'l', '%'},
    {'Q', 'E', 'T', 'U', ')', '$'},
    {'W', 'R', 'Y', 'I', '(', '#'},
    {'S', 'F', 'H', 'K', '*', '@'},
    {'X', 'V', 'N', 'O', '&', '!'},
  }
};

char GetKey4x4() {
  if (!digitalRead(A0)) return KEY_BACKSPACE;
  if (!digitalRead(A1)) return KEY_RETURN;
  if (!digitalRead(A3)) return ' ';
  char key = 0;
  char k = digitalRead(A2) ? 0 : 1;
  for (byte p = 0; p < 6; p++) {
    digitalWrite(P[p], LOW);
    for (byte m = 0; m < 6; m++) {
      if (!digitalRead(M[m])) {
        key = k6x6[k][p][m];
      }
    }
    digitalWrite(P[p], HIGH);
  }
  return key;
}

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(P[i], OUTPUT);
    if (i < 6) pinMode(M[i], INPUT_PULLUP);
    digitalWrite(P[i], HIGH);
  }
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  delay(100);
  key = GetKey4x4();
  if (key != 0) {
    pkey = key;
    if (key != -1)
      Keyboard.print(key);
  }
}
