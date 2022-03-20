#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <stdint.h>
#include <thread>
#include "beeper.h"
#define HIGH 1
#define LOW 0
#define OZEMU
#define PROGMEM
Beeper beeper;

uint32_t millis(){
	return (uint32_t)clock();
}

uint32_t micros(){
	return (uint32_t)clock();
}

uint32_t analogRead(uint32_t _){return 0;}
void analogWrite(uint8_t _, uint32_t __){return;}
uint8_t pgm_read_byte(uint32_t _){return 0;}

unsigned int random(unsigned int max){
	return rand() % max;
}

void randomSeed(unsigned int _){
	srand(static_cast<unsigned int>(time(0)));
}

void delay(unsigned int t){
	usleep(t*1000);
}

void _next_tone(){
	beeper.stop();
}

void (*next_tone)(void) = *_next_tone;

void beep(uint16_t freq, int32_t dur){
	beeper.setFrequency(freq);
	usleep(1000*dur);
	if (dur) thread(next_tone).detach();
}

void tone(uint8_t _, uint16_t freq, uint16_t dur) {
	beeper.play();
	thread([](uint16_t x, uint16_t y) { beep(x, y); }, freq, dur).detach();
}

void tone(void (*f)(void)) {
    next_tone = *f;
	next_tone();
}

void noTone(uint8_t _) {
    next_tone = *_next_tone;
    next_tone();
}
