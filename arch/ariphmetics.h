void add_rr() {
  readRegisters();
  R[r1] += R[r2];
}

void add_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = R[r1] + *(float*)&x;
}

void sub_rr() {
  readRegisters();
  R[r1] -= R[r2];
}

void sub_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] -= *(float*)&x;
}
  
void mul_rr() {
  readRegisters();
  R[r1] *= R[r2];
}

void mul_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] *= *(float*)&x;
}

void div_rr() {
  readRegisters();
  R[r1] /= R[r2];
}

void div_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] /= *(float*)&x;
}

void pow_rr() {
  readRegisters();
  R[r1] = pow(R[r1], R[r2]);
}

void pow_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = pow(R[r1], *(float*)&x);
}

void cmp_rr() {
  readRegisters();
  setFlags(R[r1] - R[r2]);
}

void cmp_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  setFlags(R[r1] - *(float*)&x);
}

void mod_rr() {
  readRegisters();
  R[r1] = (uint32_t)R[r1] % (uint32_t)R[r2];
}

void mod_rc() {
  readRegisters();
  uint32_t x = readNum(4);
  R[r1] = (uint32_t)R[r1] % (uint32_t)(*(float*)&x);
}
