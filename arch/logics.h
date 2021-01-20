void and_rr() {
  readRegisters();
  R[r1] = (uint32_t)R[r1] & (uint32_t)R[r2];
}

void and_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = (uint32_t)R[r1] & (uint32_t)(*(float*)&x);
}

void or_rr() {
  readRegisters();
  R[r1] = (uint32_t)R[r1] | (uint32_t)R[r2];
}

void or_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = (uint32_t)R[r1] | (uint32_t)(*(float*)&x);
}

void xor_rr() {
  readRegisters();
  R[r1] = (uint32_t)R[r1] ^ (uint32_t)R[r2];
}

void xor_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = (uint32_t)R[r1] ^ (uint32_t)(*(float*)&x);
}
