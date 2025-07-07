#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <stdint.h>
#include <thread>
#include "beeper.h"
#define HIGH 1
#define LOW 0
#define A1 15
#define A4 18
#define OZEMU
#define PROGMEM
#define OUTPUT 0
#define FALLING 0
Beeper beeper;
thread beep_thr = thread([]() { });

uint32_t millis(){ return (uint32_t)(clock() / (CLOCKS_PER_SEC/1000)); }
uint32_t micros(){ return (uint32_t)(clock() / (CLOCKS_PER_SEC/1000000)); }
uint32_t analogRead(int) {return 0;}
void analogWrite(int, int) {}
uint8_t pgm_read_byte(const uint8_t* _) {return 0;}
void digitalWrite(int, int) {}
void pinMode(int, int) {}
void attachInterrupt(int, void (*f)(void), int) { thread(f).detach(); }
unsigned int random(unsigned int max){ return rand() % max; }
void randomSeed(int){ srand((uint32_t)time(0)); }
void delay(unsigned int t){ usleep(t * 1000); }
void _next_tone() { beeper.stop(); }
void (*next_tone)(void) = *_next_tone;

void beep(uint16_t freq, int32_t dur){
	beeper.setFrequency(freq);
	usleep(1000 * dur);
	if (dur) thread(next_tone).detach();
}

void tone(uint8_t _, uint16_t freq, uint16_t dur) {
	beep_thr.join();
	beeper.play();
	beep_thr = thread(beep, freq, dur);
}

void tone(void (*f)(void)) {
	next_tone = *f;
	next_tone();
}

void noTone(uint8_t _) {
	next_tone = *_next_tone;
	next_tone();
}
