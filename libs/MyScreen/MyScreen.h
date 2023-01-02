#ifndef PCD8544_H
#define PCD8544_H

#include <Arduino.h>
#define pin_sclk 9
#define pin_sdin 4
#define pin_dc 5
#define pin_reset 7
#define pin_sce 6
#define PCD8544_CMD  LOW
#define PCD8544_DATA HIGH
#define width 84
#define height 48
#define bufsize 504

class PCD8544: public Print {
	public:
		uint16_t cursor = 512;
		uint8_t screen_buffer[bufsize];
		// All the pins can be changed from the default values...
		void begin();

		// Set display contrast level (0-127)...
		void setContrast(uint8_t level);

		// Place the cursor at position (column, line)...
		void setCursor(uint8_t column, uint8_t line);

		// Write an ASCII character at the current cursor position (7-bit)...
		virtual size_t write(uint8_t chr);

		// Send a command or data to the display...
		void send(uint8_t type, uint8_t data);
		
		bool draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi);
		
		void draw_circle(int16_t X1, int16_t Y1, int16_t y);
		
		void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
		
		bool set_point(int16_t x, int16_t y);
		
		void clear();
		
		void reverse(uint16_t from, uint16_t to);
			
		void setColor(uint8_t c);

	private:
		bool set_data(uint8_t data, uint16_t i);
		
		void draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy);
		
		void setColored(uint16_t i, uint8_t data);
};
#endif
