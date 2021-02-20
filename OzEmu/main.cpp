#include "libs/Arduino.h"
#include "libs/sd_raw.h"
#include "libs/PS2Keyboard.h"
#include "libs/MyScreen.h"
#include <iostream>
using namespace std;
#include "../arch/arch.h"

int main(){
	if (!sd_raw_init()) while(true);
	lcd.begin();
	keyboard.begin(8, 3);
	randomSeed(analogRead(0));
	lcd.setCursor(0, 0);
	while (true)
		comms[read_()]();
	endwin();
	return 0;
}
