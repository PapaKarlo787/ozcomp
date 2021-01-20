#include "MyScreen.h"

#include <Arduino.h>

#if defined (__XTENSA__)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif


#define PCD8544_CMD  LOW
#define PCD8544_DATA HIGH
#define width 84
#define height 48

/*
 * If this was a ".h", it would get added to sketches when using
 * the "Sketch -> Import Library..." menu on the Arduino IDE...
 */
#include "charset.cpp"

void PCD8544::begin()
{
	// All pins are outputs (these displays cannot be read)...
    pinMode(this->pin_sclk, OUTPUT);
    pinMode(this->pin_sdin, OUTPUT);
    pinMode(this->pin_dc, OUTPUT);
    pinMode(this->pin_reset, OUTPUT);
    pinMode(this->pin_sce, OUTPUT);

    // Reset the controller state...
    digitalWrite(this->pin_reset, HIGH);
    digitalWrite(this->pin_sce, HIGH);
    digitalWrite(this->pin_reset, LOW);
    delay(100);
    digitalWrite(this->pin_reset, HIGH);

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
    this->send(PCD8544_CMD, 0x80 | (column % 84));
    this->send(PCD8544_CMD, 0x40 | (line % 6));
}

size_t PCD8544::write(uint8_t chr)
{
    // ASCII 7-bit only...
    if (chr >= 0x80 || chr < 0x20)
        chr = ' ';

    const uint8_t *glyph;
    uint8_t pgm_buffer[5];

	// Regular ASCII characters are kept in flash to save RAM...
	memcpy_P(pgm_buffer, &charset[chr - ' '], sizeof(pgm_buffer));
	glyph = pgm_buffer;

    // Output one column at a time...
    for (uint8_t i = 0; i < 5; i++)
        this->send(PCD8544_DATA, glyph[i]);

    // One column between characters...
    this->send(PCD8544_DATA, 0x00);

    return 1;
}

void PCD8544::send(uint8_t type, uint8_t data)
{
    digitalWrite(this->pin_dc, type);
    digitalWrite(this->pin_sce, LOW);
    shiftOut(this->pin_sdin, this->pin_sclk, MSBFIRST, data);
    digitalWrite(this->pin_sce, HIGH);
}


/* vim: set expandtab ts=4 sw=4: */
