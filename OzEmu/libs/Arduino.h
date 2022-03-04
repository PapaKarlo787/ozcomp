#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <stdint.h>
#define HIGH 1
#define LOW 0
#define OZEMU
#define PROGMEM

uint32_t millis(){
	return (uint32_t)clock();
}

uint32_t micros(){
	return (uint32_t)clock();
}

unsigned int analogRead(uint8_t _){}
unsigned int analogWrite(uint8_t _, uint16_t __){}
unsigned int pgm_read_byte(uint16_t _){}

unsigned int random(unsigned int max){
	return rand() % max;
}

void randomSeed(unsigned int _){
	srand(static_cast<unsigned int>(time(0)));
}

void delay(unsigned int t){
	usleep(t*1000);
}


#include "beeper.h"
