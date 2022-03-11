#include <fstream>
#include <thread>
#include <cstdlib>


ofstream music_dev("/dev/ttyUSB0");
thread beep_thr = thread([]() { });
bool is_plaing = false;

void _next_tone(){
	is_plaing = false;
	uint32_t v = 0;
	music_dev.write((const char*)&v, sizeof(v));
}

void (*next_tone)(void) = *_next_tone;

void beep(uint16_t freq, uint16_t dur){
	music_dev.write((const char*)&freq, sizeof(freq));
	music_dev.write((const char*)&dur, sizeof(dur));
	if (dur) delay(dur);
	else while (is_plaing);
	thread(next_tone).detach();
}

void tone(uint8_t _, uint16_t freq, uint16_t dur) {
	is_plaing = false;
	beep_thr.join();
	is_plaing = true;
	beep_thr = thread([](uint16_t x, uint16_t y) { beep(x, y); }, freq, dur);
}

void tone(void (*f)(void)) {
    next_tone = *f;
	next_tone();
}

void noTone(uint8_t _) {
    next_tone = *_next_tone;
    next_tone();
}
