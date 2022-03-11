class PS2Keyboard {
  public:
    PS2Keyboard();
    static bool available();
    static void begin();
    static uint8_t read();
  private:
	
};
