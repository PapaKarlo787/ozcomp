#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h>
#define HIGH 1
#define LOW 0

unsigned int analogRead(unsigned int _){}

unsigned int random(unsigned int max){
	return rand();
}

void randomSeed(unsigned int _){
	srand(static_cast<unsigned int>(time(0)));
}

void delay(unsigned int t){
	usleep(t*1000);
}
