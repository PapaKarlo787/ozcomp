class PS2Keyboard {
  public:
    PS2Keyboard();
    static void begin(uint8_t dataPin, uint8_t irq_pin);
    static bool available();
    static int read();
};
