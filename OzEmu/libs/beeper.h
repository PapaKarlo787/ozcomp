#include <fstream>
#include <thread>


ofstream music_pipe("pipe");
thread beep_thr;
bool is_plaing = false;

void _next_tone(){
	is_plaing = false;
}

void (*next_tone)(void) = *_next_tone;

void beep(uint16_t freq, uint16_t dur){
	for (int i = 0; i < dur * 88; ++i) {
		if (!is_plaing)
			return;
		int16_t a = i % (44100 / freq) > (44100 / freq / 2) ? 32000 : -32000;
		music_pipe.write((const char *)&a, sizeof(a));
	}
	next_tone();
}

void tone(uint8_t _, uint16_t freq, uint16_t dur) {
	beep_thr = thread([](uint16_t x, uint16_t y) { beep(x, y); }, freq, dur);
	beep_thr.detach();
	is_plaing = true;
}

void tone(void (*f)(void)) {
    next_tone = *f;
	next_tone();
}

void noTone(uint8_t _) {
    next_tone = *_next_tone;
    next_tone();
}
