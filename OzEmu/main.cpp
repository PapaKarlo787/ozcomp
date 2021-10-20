#include <iostream>
#include "libs/Arduino.h"
#include "libs/sd_raw.h"
#include "libs/PS2Keyboard.h"
#include "libs/MyScreen.h"
#include "libs/beeper.h"
using namespace std;
#include "../arch/arch.h"

int main(){
	if (!sd_raw_init()) while(true);
	lcd.begin();
	keyboard.begin(3, 8); // kbd and lcd order of init is essential
	init_beeper();
	randomSeed(analogRead(0));
	while (true){
		int x = read_();
		comms[x]();
	}
	//sp
	endwin();
	clean_up(0);
	return 0;
}
