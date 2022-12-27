#include "MyScreen.h"
#include "charset.cpp"
#define PCD8544_CMD  LOW
#define PCD8544_DATA HIGH
#define width 84
#define height 48

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
        this->send(PCD8544_DATA, screen_buffer[cursor]);
	}

    // One column between characters...
    this->send(PCD8544_DATA, 0x00);
    return 1;
}

void PCD8544::send(uint8_t type, uint8_t data)
{
    PORTD &= ~(0b11 << pin_dc);
    PORTD |= type << pin_dc;
    shiftOut(pin_sdin, pin_sclk, MSBFIRST, data);
    PORTD |= 1 << pin_sce;
    if (type)
		cursor = (cursor + 1) % 504;
}
