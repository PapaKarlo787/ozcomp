#ifndef PCD8544_H
#define PCD8544_H

#include <Arduino.h>
#define pin_sclk 9
#define pin_sdin 4
#define pin_dc 5
#define pin_reset 7
#define pin_sce 6

class PCD8544: public Print {
    public:
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
};
#endif
