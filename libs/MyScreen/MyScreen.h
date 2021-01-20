#ifndef PCD8544_H
#define PCD8544_H

#include <Arduino.h>

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

    private:
        uint8_t pin_sclk = 9;
        uint8_t pin_sdin = 4;
        uint8_t pin_dc = 5;
        uint8_t pin_reset = 7;
        uint8_t pin_sce = 6;
};
#endif  /* PCD8544_H */


/* vim: set expandtab ts=4 sw=4: */
