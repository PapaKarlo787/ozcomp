#include <Keyboard.h>

const int P[] = {8,9,15,14,16,10};
const int M[] = {2,3,4,5,6,7};
const char k6x6 [4][6][6] = {
  {
    {'1', '2', '3', 'A', 'B', 'C'},
    {'4', '5', '6', 'D', 'B', 'B'},
    {'7', '8', '9', 'C', 'E', 'F'},
    {'*', '0', '#', 'G', 'H', 'I'},
    {'!', '(', ')', 'J', 'K', 'L'},
    {'+', '-', '=', 'M', 'N', 'O'},
  },
  {
    {'1', '2', '3', 'A', 'B', 'C'},
    {'4', '5', '6', 'D', 'B', 'B'},
    {'7', '8', '9', 'C', 'E', 'F'},
    {'*', '0', '#', 'G', 'H', 'I'},
    {'!', '(', ')', 'J', 'K', 'L'},
    {'+', '-', '=', 'M', 'N', 'O'},
  },
  {
    {'1', '2', '3', 'A', 'B', 'C'},
    {'4', '5', '6', 'D', 'B', 'B'},
    {'7', '8', '9', 'C', 'E', 'F'},
    {'*', '0', '#', 'G', 'H', 'I'},
    {'!', '(', ')', 'J', 'K', 'L'},
    {'+', '-', '=', 'M', 'N', 'O'},
  },
  {
    {'1', '2', '3', 'A', 'B', 'C'},
    {'4', '5', '6', 'D', 'B', 'B'},
    {'7', '8', '9', 'C', 'E', 'F'},
    {'*', '0', '#', 'G', 'H', 'I'},
    {'!', '(', ')', 'J', 'K', 'L'},
    {'+', '-', '=', 'M', 'N', 'O'},
  }
};

char GetKey4x4() {
  static unsigned long timer;
  static char olda;
  char a = 0;
  char k = 0;
  if ((timer + 50) > millis()) return 0;  // пауза для опроса передает 1, сам опрос возврвщает 0 если не нажато или символ, если нажато
  for (byte p = 0; p < 6; p++) {    // последовательно выставляем по одной строке в LOW
    digitalWrite(P[p], LOW);
    for (byte m = 0; m < 6; m++) {  // и считываем столбцы вылавнивая где LOW происходит
      if (!digitalRead(M[m])) {
        a = k6x6[p][m][k];             // считываем соотвествующий символ для комбинации столбца и строки
      }
    }
    digitalWrite(P[p], HIGH);       // возвращем строку в HIGH и крутим дальше
  }
  timer = millis();
  return a;
}

void setup() {

  for (int i = 0; i < 6; i++) {
    pinMode(P[i], OUTPUT);
    pinMode(M[i], INPUT_PULLUP);
    digitalWrite(P[i], HIGH);
  }
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  Serial.begin(9600);
}

void loop() {

  char a = GetKey4x4();             // опрашиваем клавиатуру
  if (a != 0) {                     // если кнопка была нажата выводим ее в порт
    Serial.print(a);
  }
}
