#define MBS 8

uint32_t play_pointer;
uint32_t start_pointer;
uint32_t size;
uint8_t mhead;
uint8_t mtail;
uint32_t mbuffer[MBS];

void fill_mbuffer() {
	if (!start_pointer) return;
	while((mtail - mhead + 1) % MBS){
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
	if (flags & ((uint32_t)1 << 17)) {
		digitalWrite(Rp, freq & 128);
		digitalWrite(Gp, freq & 512);
		digitalWrite(Bp, freq & 2048);
	}
	if (freq == -1)
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
