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
	kbd_begin();
	randomSeed(analogRead(0));
	while (true){
		comms[read_()]();
	}
	//sp
	endwin();
	clean_up(0);
	return 0;
}
