#ifndef NOKIA_SCREEN
#define NOKIA_SCREEN

#include <Arduino.h>
#define pin_reset 7
#define pin_sce 6
#define width 84
#define height 48
#define bufsize 504

class NokiaScreen: public Print {
	public:
		uint16_t cursor = 512;
		uint8_t screen_buffer[bufsize];
		// All the pins can be changed from the default values...
		void begin();

		// Set display contrast level (0-127)...
		void setContrast(uint8_t level);

		// Place the cursor at position (column, line)...
		void setCursor(uint8_t column, uint8_t line);
		void setCursor(uint16_t ind);

		// Write an ASCII character at the current cursor position (7-bit)...
		virtual size_t write(uint8_t chr);

		// Send a command or data to the display...
		void send_data(uint8_t data);
		void send_cmd(uint8_t data);
		
		bool draw_bmp(int8_t x, int8_t y, uint8_t (*reader) (), uint32_t* poi);
		
		void draw_circle(int16_t X1, int16_t Y1, int16_t y);
		
		void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
		
		bool set_point(int16_t x, int16_t y);
		
		void clear();
		
		void rst();
		
		void reverse(uint16_t from = 0, uint16_t to = bufsize);
			
		void setColor(uint8_t c);

	private:
		bool set_data(uint8_t data, uint16_t i);
		
		void draw_line(int16_t x, int16_t y, uint16_t dx, uint16_t dy, int8_t sx, int8_t sy, int8_t sxx, int8_t syy);
		
		void setColored(uint16_t i, uint8_t data);
};
#endif
