void get_key() {
	setFlags(R[15] = kbd.take());
}

void kread() {
	R[read_()] = kbd.read();
}

void kwrite() {
	kbd.write(R[read_()]);
}
