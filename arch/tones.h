uint32_t play_pointer;
uint32_t start_pointer;
uint32_t size;
const uint8_t cols[] PROGMEM = { 7, 18, 15, 20, 2, 8, 21, 6, 19, 11, 24, 5, 26 };

void next_play(){
	uint32_t freq = readNum(play_pointer+start_pointer, 1);
	tone(15, freq, readNum(play_pointer+start_pointer+2, 2));
	play_pointer = (play_pointer + 4) % size;
	uint8_t note = freq > 27 ? (int8_t)(log(freq / 27.5) / log(1.05946309436)) % 12 : 12;
	note = pgm_read_byte(&cols[note]);
	analogWrite(Rp, (note % 3) * 511);
	analogWrite(Gp, (note / 3 % 3) * 511);
	analogWrite(Bp, (note / 9 % 3) * 511);
}

void play(){
	uint32_t poi = readNum();
	start_pointer = poi+4;
	play_pointer = 0;
	size = readNum(poi, 4) << 2;
	tone(next_play);
}

void nplay(){
	noTone(15);
}
