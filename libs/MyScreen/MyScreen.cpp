#include "MyScreen.h"
#include "charset.cpp"

void PCD8544::begin()
{
	// All pins are outputs (these displays cannot be read)...
    pinMode(pin_sclk, OUTPUT);
    pinMode(pin_sdin, OUTPUT);
    pinMode(pin_dc, OUTPUT);
    pinMode(pin_reset, OUTPUT);
    pinMode(pin_sce, OUTPUT);

    // Reset the controller state...
    digitalWrite(pin_reset, HIGH);
    digitalWrite(pin_sce, HIGH);
    digitalWrite(pin_reset, LOW);
    delay(100);
    digitalWrite(pin_reset, HIGH);

    // Set the LCD parameters...
    this->send(PCD8544_CMD, 0x21);  // extended instruction set control (H=1)
    this->send(PCD8544_CMD, 0x13);  // bias system (1:48)
    this->send(PCD8544_CMD, 0xc2);  // default Vop (3.06 + 66 * 0.06 = 7V)
    this->send(PCD8544_CMD, 0x20);  // extended instruction set control (H=0)
    this->send(PCD8544_CMD, 0x09);  // all display segments on

    // Activate LCD...
    this->send(PCD8544_CMD, 0x08);  // display blank
    this->send(PCD8544_CMD, 0x0c);  // normal mode (0x0d = inverse mode)
    delay(100);

    // Place the cursor at the origin...
    this->send(PCD8544_CMD, 0x80);
    this->send(PCD8544_CMD, 0x40);
}

void PCD8544::setContrast(uint8_t level)
{
    // The PCD8544 datasheet specifies a maximum Vop of 8.5V for safe
    // operation in low temperatures, which limits the contrast level.
    if (level > 90)
        level = 90;  // Vop = 3.06 + 90 * 0.06 = 8.46V

    this->send(PCD8544_CMD, 0x21);  // extended instruction set control (H=1)
    this->send(PCD8544_CMD, 0x80 | (level & 0x7f));
    this->send(PCD8544_CMD, 0x20);  // extended instruction set control (H=0)
}

void PCD8544::setCursor(uint8_t column, uint8_t line)
{
	column = (column % width);
	line = (line % 6);
	cursor = line * width + column;
    this->send(PCD8544_CMD, 0x80 | column);
    this->send(PCD8544_CMD, 0x40 | line);
}

size_t PCD8544::write(uint8_t chr)
{
    // Output one column at a time...
    for (uint8_t i = 0; i < 5; i++) {
		screen_buffer[cursor & 511] = pgm_read_word(&charset[chr][i]);
        this->send(PCD8544_DATA, screen_buffer[cursor & 511]);
	}

    // One column between characters...
    this->send(PCD8544_DATA, screen_buffer[cursor & 511] = 0x00);
    return 1;
}

void PCD8544::send(uint8_t type, uint8_t data)
{
    PORTD &= ~(0b11 << pin_dc);
    PORTD |= type << pin_dc;
    shiftOut(pin_sdin, pin_sclk, MSBFIRST, data);
    PORTD |= 1 << pin_sce;
    screen_buffer[cursor & 511] = data;
    if (type)
		cursor = (cursor & 0xfe00) + ((cursor & 0x1ff) + 1) % bufsize;
}

bool PCD8544::set_point(int16_t x, int16_t y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return 0;
	uint16_t n = y / 8 * width + x;
	uint8_t tmp = screen_buffer[n];
	setColored(n, 1 << y % 8);
	setCursor(x, y/8);
	send(HIGH, screen_buffer[n]);
	return tmp == screen_buffer[n];
}

void PCD8544::draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy) {
	int16_t d = (dx << 1) - dy;
	int16_t d1 = dx << 1;
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

void PCD8544::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
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

void PCD8544::draw_circle(int16_t X1, int16_t Y1, int16_t y) {
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

bool PCD8544::set_data(uint8_t data, uint16_t i) {
	uint8_t last = screen_buffer[i];
	setColored(i, data);
	setCursor(i % width, i / width);
	send(HIGH, screen_buffer[i]);
	return last + data != screen_buffer[i];
}

bool PCD8544::draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi) {
	uint8_t sx = reader();
	uint8_t sy = reader();
	uint8_t shift = (8 - y % 8) % 8;
	bool intersected = false;
	if (y < 0) y -= 8;
	y = y / 8 + (y % 8 ? 1 : 0);
	int start = y * width + x;
	for (uint8_t i = 0; i < sy; i++) {
		if (i + y > -1 && i + y < 7){
			for (uint8_t l = 0; l < sx; l++) {
				if (l + x > -1 && l + x < width) {
					uint16_t c = (uint16_t)reader() * 256 >> shift;
					if (i + y > 0)
						intersected |= set_data(c % 256, start+l-width);
					if (i + y < 6)
						intersected |= set_data((c / 256) % 256, start+l);
				} else (*poi)++;
			}
		} else (*poi) += sx;
		start += width;
	}
	return intersected;
}

void PCD8544::setColored(uint16_t i, uint8_t data) {
	if (cursor & 512)
		screen_buffer[i] |= data;
	else
		screen_buffer[i] &= ~data;
}

void PCD8544::clear() {
	setCursor(0, 0);
	for(uint16_t i = 0; i < bufsize; i++)
		send(HIGH, 0);
}

void PCD8544::reverse(uint8_t from, uint8_t to) {
	if (from >= bufsize) return;
	setCursor(from % width, from / width);
	for(uint16_t i = from; i < to; i++)
		send(HIGH, ~screen_buffer[i]);
}

