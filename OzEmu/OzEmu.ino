void setup(){
	Serial.begin(9600);
}

void loop(){
	if (Serial.available()) {
		uint16_t freq = Serial.read();
		freq += Serial.read() << 8;
		uint16_t dur = Serial.read();
		dur += Serial.read() << 8;
		if (freq)
			tone(5, freq, dur);
		else
			noTone(5);
	}
}
