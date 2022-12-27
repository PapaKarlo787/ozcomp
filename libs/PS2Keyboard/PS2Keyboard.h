class PS2Keyboard {
  public:
    PS2Keyboard();
    static void begin();
    static uint8_t read();
    static uint8_t take();
	static void write(uint8_t data);
  private:
};
