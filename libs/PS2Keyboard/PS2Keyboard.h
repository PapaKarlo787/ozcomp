class PS2Keyboard {
  public:
    PS2Keyboard();
    static bool available();
    static void begin(uint8_t dataPin);
    static uint8_t read();
  private:
	
};
