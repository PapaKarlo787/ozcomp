#define MBS 8

uint32_t play_pointer;
uint32_t start_pointer;
uint32_t size;
uint8_t mhead;
uint8_t mtail;
uint32_t mbuffer[MBS];
const uint8_t cols[] PROGMEM = { 9, 32, 24, 34, 2, 10, 36, 8, 33, 18, 40, 6, 42 };

void fill_mbuffer() {
	if (!start_pointer) return;
	while((mtail-mhead+1) % MBS){
		mbuffer[mtail] = readNum(play_pointer+start_pointer);
		play_pointer = (play_pointer + 4) % size;
		mtail = (mtail + 1) % MBS;
	}
}

void next_play(){
	if (mhead == mtail) return;
	uint32_t sig = mbuffer[mhead];
	mhead = (mhead + 1) % MBS;
	uint16_t freq = sig & 0xffff;
	tone(17, freq, sig >> 16);
#ifndef V1
	if (flags & ((uint32_t)1 << 17)) {
		uint8_t note = freq > 27 ? ((int16_t)(log(freq / 27.5) / log(1.05946309436))) % 12 : 12;
		note = pgm_read_byte(&(cols[note]));
		analogWrite(Rp, (note & 3) * 511);
		analogWrite(Gp, ((note >> 2) & 3) * 511);
		analogWrite(Bp, ((note >> 4) & 3) * 511);
	}
#endif
	if (freq == 0xffff)
		noTone(17);
}

void play(){
	uint32_t poi = readNum();
	start_pointer = poi+4;
	play_pointer = 0;
	size = readNum(poi) << 2;
	mhead = 0;
	mtail = 0;
	fill_mbuffer();
	tone(next_play);
}

void nplay(){
	noTone(17);
	start_pointer = 0;
}
