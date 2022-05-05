using namespace std;
#include <iostream>
#include "libs/Arduino.h"
#include "libs/sd_raw.h"
#include "libs/MyScreen.h" // kbd and lcd order of init is essential
#include "libs/ps2.h"
#include "libs/RTC.h"
#include "../arch/arch.h"

int main(){
	SDL_Init(SDL_INIT_AUDIO);
	beeper.open();
	beeper.setVolume(1.0);
	cout << "This emulator not supported under X Window System.\nwait for 3 seconds or break..." << endl;
	cout << "Clocs per second " << CLOCKS_PER_SEC << endl;
	delay(3000);
	if (!sd_raw_init()) while(true);
	lcd.begin();
	timeUnix.begin();
	keyboard.begin(); // kbd and lcd order of init is essential!!!!!!!
	randomSeed(analogRead(0));
	sp = 1 << 31;
	while (true){
		int x = read_();
		comms[x]();
		fill_mbuffer();
	}
	SDL_Quit();
	return 0;
}
