void setup(){
	Serial.begin(9600);
}

uint16_t read_num(){
	while (!Serial.available());
	uint16_t res = Serial.read();
	while (!Serial.available());
	res += Serial.read() << 8;
	return res;
}

void loop(){
	uint16_t freq = read_num();
	uint16_t dur = read_num();
	if (freq) tone(6, freq, dur);
	else noTone(6);
}
