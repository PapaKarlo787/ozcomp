uint32_t play_pointer;
uint32_t start_pointer;
bool is_playing;
uint32_t size;
uint32_t start_tone;
uint32_t freq;
uint32_t duration;

void start_play(uint32_t poi){
	start_pointer = poi+4;
	play_pointer = 0;
	size = readNum(poi) << 3;
	is_playing = true;
	start_tone = millis();
}

void next_play(){
	if (is_playing && millis() - start_tone >= duration){
		uint32_t temp = start_pointer + play_pointer;
		freq = readNum(temp);
		duration = readNum(temp+4);
		noTone(15);
		tone(15, freq);
		play_pointer = (play_pointer + 8) % size;
		start_tone = millis();
	}
}

void play(){
	start_play(readNum());
}

void nplay(){
	noTone(15);
	is_playing = false;
}
