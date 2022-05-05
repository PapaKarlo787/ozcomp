void get_key() {
	keyboard.write(0);
	setFlags(R[15] = keyboard.read());
	delay(2);
}

void get_mice() {
	keyboard.write(1);
	R[15] = 0;
	for (uint8_t i = 0; i < 24; i+=8)
		R[15] |= keyboard.read() << i;
	setFlags(R[15]);
	delay(2);
}

void get_joy() {
	keyboard.write(2);
	setFlags(R[15] = keyboard.read());
	delay(2);
}

void get_IR() {
	keyboard.write(3);
	setFlags(R[15] = keyboard.read());
	delay(2);
}
