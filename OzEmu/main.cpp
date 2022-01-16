using namespace std;
#include <iostream>
#include "libs/Arduino.h"
#include "libs/sd_raw.h"
#include "libs/MyScreen.h" // kbd and lcd order of init is essential
#include "libs/PS2Keyboard.h"
#include "libs/beeper.h"
#include "../arch/arch.h"

int main(){
	if (!sd_raw_init()) while(true);
	lcd.begin();
	keyboard.begin(3, 8); // kbd and lcd order of init is essential
	randomSeed(analogRead(0));
	while (true){
		int x = read_();
		comms[x]();
	}
	//sp
	return 0;
}
