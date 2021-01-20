void get_key() {
  R[15] = keyboard.available() ? keyboard.read() : 0;
}

void print_line() {
  uint32_t temp = ip;
  ip = readNum(4);
  while (uint8_t c = read())
    lcd.write(c);
  ip = temp + 4;
}

void print_num() {
  readRegisters();
  lcd.print(R[r1]);
}

void print_int() {
  readRegisters();
  lcd.print((int)R[r1]);
}

void snd_r() {
  readRegisters();
  lcd.send(HIGH, (uint8_t)R[r1]);
}

void snd_c() {
  lcd.send(HIGH, (uint8_t)readNum(1));
}

void set_cursor() {
  lcd.setCursor((uint8_t)readNum(1), (uint8_t)readNum(1));
}

void set_cursor_r() {
  readRegisters();
  lcd.setCursor((uint32_t)R[r1], (uint32_t)R[r2]);
}

void draw_screen() {
  uint32_t temp = ip;
  ip = readNum(4);
  for (uint32_t i = 0; i < 504; i++)
    lcd.send(HIGH, (uint8_t)readNum(1));
  ip = temp + 4;
}
