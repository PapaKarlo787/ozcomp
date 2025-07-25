#include "MyScreen.h"
#include "charset.cpp"

void NokiaScreen::begin() {
	DDRD |= 0xF0;
	DDRB |= 2;

    digitalWrite(pin_reset, LOW);
    digitalWrite(pin_reset, HIGH);
    digitalWrite(pin_sce, HIGH);
    
    send_cmd(0x21);  // extended instruction set control (H=1)
    send_cmd(0x13);  // bias system (1:48)
    send_cmd(0xc2);  // default	Vop (3.06 + 66 * 0.06 = 7V); other ver 0xb8
    send_cmd(0x20);  // extended instruction set control (H=0)
    send_cmd(0x09);  // all display segments on
    send_cmd(0x08);  // display blank
    send_cmd(0x0c);  // normal mode (0x0d = inverse mode)
    send_cmd(0x80);  // 0 column
    send_cmd(0x40);  // 0 row
}

void NokiaScreen::setContrast(uint8_t level) {
    this->send_cmd(0x21);  // extended instruction set control (H=1)
    this->send_cmd(0x80 | (min(level, 90) & 0x7f));
    this->send_cmd(0x20);  // extended instruction set control (H=0)
}

void NokiaScreen::setCursor(uint8_t column, uint8_t line) {
	cursor = (cursor & 0xfe00) + line * width + column;
    send_cmd(0x80 | column);
    send_cmd(0x40 | line);
}

void NokiaScreen::setCursor(uint16_t ind) {
	cursor = (cursor & 0xfe00) + ind;
    send_cmd(0x80 | (ind % width));
    send_cmd(0x40 | (ind / width));
}

size_t NokiaScreen::write(uint8_t chr) {
    for (uint8_t i = 0; i < 5; i++) {
        send_data(pgm_read_word(&charset[chr][i]));
	}
    send_data(0x00);
}

void NokiaScreen::send_data(uint8_t data) {
    PORTD |= 32;
    for(int i = 0; i < 8; i++) {
		PORTD &= ~16;
		PORTD |= ((data >> i) & 1) << 4;
		PORTB |= 2;
		PORTB &= ~2;
	}
	screen_buffer[cursor & 511] = data;
	cursor = (cursor & 0xfe00) + ((cursor & 0x1ff) + 1) % bufsize;
}

void NokiaScreen::send_cmd(uint8_t cmd) {
    PORTD &= 223;
    for(int i = 0; i < 8; i++) {
		PORTD &= ~16;
		PORTD |= ((cmd >> i) & 1) << 4;
		PORTB |= 2;
		PORTB &= ~2;
	}
}

void NokiaScreen::rst(){
	setCursor(0, 0);
	PORTD |= 32;
	for(uint16_t i = 0; i < bufsize; i++) {
		for(int l = 0; l < 8; l++) {
			PORTD &= ~16;
			PORTD |= ((screen_buffer[i] >> l) & 1) << 4;
			PORTB |= 2;
			PORTB &= ~2;
		}
	}
}

void NokiaScreen::setColor(uint8_t c){
	cursor = (cursor & 512) | c << 9;
}

void NokiaScreen::setColored(uint16_t i, uint8_t data) {
	if (cursor & 512)
		screen_buffer[i] |= data;
	else
		screen_buffer[i] &= ~data;
}

bool NokiaScreen::set_data(uint8_t data, uint16_t i) {
	uint8_t last = screen_buffer[i];
	setColored(i, data);
	setCursor(i);
	send_data(screen_buffer[i]);
	return last + data != screen_buffer[i];
}

void NokiaScreen::clear() {
	setCursor(0, 0);
	for(uint16_t i = 0; i < bufsize; i++)
		send_data(0);
}

void NokiaScreen::reverse(uint16_t from, uint16_t to) {
	if (from >= bufsize) return;
	setCursor(from);
	for(uint16_t i = from; i < to; i++)
		send_data(~screen_buffer[i]);
}

bool NokiaScreen::set_point(int16_t x, int16_t y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return 0;
	uint16_t n = (y >> 3) * width + x;
	uint8_t tmp = screen_buffer[n];
	setColored(n, 1 << (y & 7));
	setCursor(x, y >> 3);
	send_data(screen_buffer[n]);
	return tmp == screen_buffer[n];
}

void NokiaScreen::draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy) {
	int16_t d1 = dx << 1;
	int16_t d = d1 - dy;
	int16_t d2 = (dx - dy) << 1;
	for (uint16_t i = 1; i <= dy; i++) {
		if (d > 0) {
			d += d2;
			x += sx;
			y += syy;
		} else d += d1;
		set_point(x, y);
		y += sy;
		x += sxx;
	}
}

void NokiaScreen::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	uint16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	uint16_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
	int8_t sx = (x1 >= x0) ? (1) : (-1);
	int8_t sy = (y1 >= y0) ? (1) : (-1);
	set_point(x0, y0);
	if (dy < dx)
		draw_line(x0 + sx, y0, dy, dx, 0, 0, sx, sy);
	else
		draw_line(x0, y0 + sy, dx, dy, sx, sy, 0, 0);
}

void NokiaScreen::draw_circle(int16_t X1, int16_t Y1, int16_t y) {
	int16_t x = 0;
	int16_t delta = 1 - 2 * y;
	int16_t error = 0;
	while (y >= 0) {
		set_point(X1 + x, Y1 + y);
		set_point(X1 + x, Y1 - y);
		set_point(X1 - x, Y1 + y);
		set_point(X1 - x, Y1 - y);
		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
			delta += 2 * ++x + 1;
		else if ((delta > 0) && (error > 0))
			delta -= 2 * --y + 1;
		else
			delta += 2 * (++x - --y);
	}
}

bool NokiaScreen::draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi) {
	uint8_t sx = reader();
	uint8_t sy = reader();
	uint8_t shift = (8 - y & 7) & 7;
	bool intersected = false;
	if (y < 0) y -= 8;
	y = (y >> 3) + (y & 7 ? 1 : 0);
	int start = y * width + x;
	for (uint8_t i = 0; i < sy; i++) {
		if (i + y > -1 && i + y < 7){
			for (uint8_t l = 0; l < sx; l++) {
				if (l + x > -1 && l + x < width) {
					uint16_t c = (uint16_t)reader() << (8 - shift);
					if (i + y > 0)
						intersected |= set_data(c & 0xff, start+l-width);
					if (i + y < 6)
						intersected |= set_data(c >> 8, start+l);
				} else (*poi)++;
			}
		} else (*poi) += sx;
		start += width;
	}
	return intersected;
}
