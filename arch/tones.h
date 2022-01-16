uint32_t play_pointer;
uint32_t start_pointer;
uint32_t size;

void next_play(){
	uint32_t freq = readNum(play_pointer+start_pointer, 2);
	tone(15, freq, readNum(play_pointer+start_pointer+2, 2));
	play_pointer = (play_pointer + 4) % size;
}

void play(){
	uint32_t poi = readNum();
	start_pointer = poi+4;
	play_pointer = 0;
	size = readNum(poi, 4) << 3;
	tone(next_play);
}

void nplay(){
	noTone(15);
}
