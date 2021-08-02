#include "libs/Arduino.h"
#include "libs/sd_raw.h"
#include "libs/PS2Keyboard.h"
#include "libs/MyScreen.h"
#include "libs/beeper.h"
#include <iostream>
using namespace std;
#include "../arch/arch.h"

int main(){
	if (!sd_raw_init()) while(true);
	lcd.begin();
	init_beeper();
	keyboard.begin(8, 3);
	randomSeed(analogRead(0));
	while (true){
		uint8_t x = read_();
		comms[x]();
		check_play();
	}
	endwin();
	return 0;
}
