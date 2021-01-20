void mov_rm() {
  readRegisters();
  uint32_t temp = ip;
  ip = readNum(4);
  uint32_t x = readNum(4);
  R[r1] = *(float*)&x;
  ip = temp + 4;
}

void movb_rm() {
  readRegisters();
  uint32_t temp = ip;
  ip = readNum(4);
  R[r1] = readNum(1);
  ip = temp + 4;
}

void mov_mr() {
  readRegisters();
  write(readNum(4), (uint8_t*)&(R[r1]), 4);
}

void movb_mr() {
  readRegisters();
  uint8_t x = (uint8_t)R[r1];
  write(readNum(4), (uint8_t*)&(x), 1);
}

void mov_rr() {
  readRegisters();
  R[r1] = R[r2];
}

void mov_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = *(float*)&x;
}

void mov_rmor() {
  readRegisters();
  uint32_t temp = ip;
  ip = readNum(4)+((uint32_t)R[r2]);
  uint32_t x = readNum(4);
  R[r1] = *(float*)&x;
  ip = temp + 4;
}

void movb_rmor() {
  readRegisters();
  uint32_t temp = ip;
  ip = readNum(4)+((uint32_t)R[r2]);
  R[r1] = readNum(1);
  ip = temp + 4;
}

void mov_morr() {
  readRegisters();
  write(readNum(4)+((uint32_t)R[r2]), (uint8_t*)&(R[r1]), 4);
}

void movb_morr() {
  readRegisters();
  uint8_t x = (uint8_t)R[r1];
  write(readNum(4)+((uint32_t)R[r2]), (uint8_t*)&(x), 1);
}
