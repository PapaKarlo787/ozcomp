#include <ps2dev.h>

PS2dev kbd(14, 13);
const PROGMEM int P[] = {15, 16, 2, 3, 4, 5, 6, 7}; // пины строк
const PROGMEM int M[] = {8, 9, 10, 11, 12};   // пины столбцов
const PROGMEM uint8_t led_pins[4] = {A2, A0, A1, A3};

const PROGMEM char sym_tables [4][5][8] = {
  { // Red
    {'Q', 'W', 'A', 'S', 'Z', 'X', 0x02, 0x03}, // shift, ctrl
    {'E', 'R', 'D', 'F', 'C', 'V', 0x04, 0x05}, // alt, apple
    {'T', 'Y', 'G', 'H', 'B', 'N', 0x20, 0x07}, // space, lamp
    {'U', 'I', 'J', 'K', 'M', ',', 0x14, 0x13}, // ralt, rctrl
    {'O', 'P', 'L', '`', '.', '/', 0x12, 0x0A}  // rshift, enter
  }, 
  { // Blue
    {'q', 'w', 'a', 's', 'z', 'x', 0x02, 0x03},
    {'e', 'r', 'd', 'f', 'c', 'v', 0x04, 0x05},
    {'t', 'y', 'g', 'h', 'b', 'n', 0x20, 0x07},
    {'u', 'i', 'j', 'k', 'm', '<', 0x14, 0x13},
    {'o', 'p', 'l', '~', '>', '?', 0x12, 0x0A}
  },
  { // Green
    {0x01, 0x15, '1', '2',  '[',  ']', 0x02, 0x03}, 
    {0x16, 0x17, '3', '4', '\\',  ';', 0x04, 0x05},
    {0x18, 0x19, '5', '6', '\'',  '-', 0x20, 0x07},
    {0x1A, 0x1B, '7', '8',  '=', 0x7F, 0x14, 0x13}, // del
    {0x1C, 0x1D, '9', '0', 0x0B, 0x0C, 0x12, 0x0A}  // p_up, p_dn
  }, 
  { // Black
    {0x09, 0x00,  '!',  '@',  '{',  '}', 0x02, 0x03}, // tab
    {0x00, 0x00,  '#',  '$',  '|',  ':', 0x04, 0x05},
    {0x00, 0x00,  '%',  '^',  '"',  '_', 0x20, 0x07},
    {0x00,  '(',  '&',  '*',  '+', 0x0E, 0x14, 0x13}, // left
    { ')', 0x08, 0x0D, 0x06, 0x0F, 0x10, 0x12, 0x0A}  // backspace, up, printscr, down, right
  }
};

uint64_t old_keys = 0;
uint8_t leds;
char last_key = 0;
uint32_t time = 0;
uint8_t rep_time = 0;
uint8_t table_index = 0;

void setup() {
  for (int i = 0; i < 8; i++) {  // выставляем пины строк на выход, столбцов на вход
    pinMode(pgm_read_byte(&P[i]), OUTPUT);
    digitalWrite(pgm_read_byte(&P[i]), HIGH);
  }
  for (int i = 0; i < 5; i++) {  // выставляем пины строк на выход, столбцов на вход
    pinMode(pgm_read_byte(&M[i]), INPUT_PULLUP);
  }
  table_index = 0;
  digitalWrite(pgm_read_byte(&led_pins[table_index]), 1);
}

void loop() {
  if (millis() - time > 50) {
    uint64_t cur_keys = GetKey();             // опрашиваем клавиатуру
    uint64_t changes = old_keys ^ cur_keys;
    for (byte p = 0; p < 8; p++) {
      for (byte m = 0; m < 5; m++) {
        uint64_t bit = ((uint64_t)1 << ((m << 3) + p)); 
        if (changes & bit)
          if (cur_keys & bit) {
            rep_time = 11;
            last_key = pgm_read_byte(&sym_tables[table_index][m][p]);
            kbd.write(last_key);
            if (last_key == 0x07) {
              table_index = (table_index + 1) & 3;
              digitalWrite(pgm_read_byte(&led_pins[table_index]), 1);
              digitalWrite(pgm_read_byte(&led_pins[(table_index + 1) & 3]), 0);
              digitalWrite(pgm_read_byte(&led_pins[(table_index + 2) & 3]), 0);
              digitalWrite(pgm_read_byte(&led_pins[(table_index + 3) & 3]), 0);
            }
          }
          else {
            if (last_key == pgm_read_byte(&sym_tables[table_index][m][p]))
              last_key = 0;
            kbd.write(pgm_read_byte(&sym_tables[table_index][m][p]) | 0x80);
          }
      }
    }
    time = millis();
    old_keys = cur_keys;
    if (last_key) {
      rep_time--;
      if (!rep_time) {
        rep_time = 2;
        kbd.write(last_key);
      }
    }
  }
  kbd.keyboard_handle(&leds);
}

uint64_t GetKey() {
  uint64_t res = 0;
  for (byte p = 0; p < 8; p++) {    // последовательно выставляем по одной строке в LOW
    digitalWrite(pgm_read_byte(&P[p]), LOW);
    for (byte m = 0; m < 5; m++) {  // и считываем столбцы вылавнивая где LOW происходит
      if (!digitalRead(pgm_read_byte(&M[m]))) {
        res |= (uint64_t)1 << ((m << 3) + p);             // считываем соотвествующий символ для комбинации столбца и строки
      }
    }
    digitalWrite(pgm_read_byte(&P[p]), HIGH);       // возвращем строку в HIGH и крутим дальше
  }
  return res;
}
