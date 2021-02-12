void jmp() {
  ip = (uint32_t)readNum(4);
}

void jmp_c() {
  uint8_t f = (uint8_t)readNum(1);
  if((f & flags) != 0)
    jmp();
  else
    ip += 4;
}

void call() {
  uint32_t old_ip = ip + 4;
  ip = readNum(4);
  for (uint8_t i = 0; i < 4; i++) { 
    uint8_t t[1];
    t[0] = old_ip % 256;
    old_ip /= 256;
    write(sp-=1, t, 1);
  }
}

void ret() {
  ip = sp;
  ip = readNum(4);
  sp += 4;
}

void lp() {
  ip = R[14]-- > 0 ? readNum(4) : ip + 4;
}

void push() {
  readRegisters();
  write(sp -= 4, (uint8_t*)&(R[r1]), 4);
}

void pop() {
  readRegisters();
  uint32_t temp = ip;
  ip = sp;
  uint32_t x = readNum(4);
  R[r1] = *(float*)&x;
  ip = temp;
  sp += 4;
}
