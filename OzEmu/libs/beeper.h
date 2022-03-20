#include <SDL2/SDL.h>  


// This class is a singleton, which is bad practice. However, this makes the
// implementation more straightforward.
class Beeper
{
public:
    static void open(); // Open the audio device
    static void close(); // Close the audio device

    static void setFrequency(double frequency); // Units: Hz
    static void setVolume(double volume); // Range: 0.0 .. 1.0

    static void play();
    static void stop();

    static SDL_AudioSpec m_obtainedSpec;

private:
    static SDL_AudioDeviceID m_audioDevice;
    static double m_frequency; // Units: Hz
    static double m_volume; // Range: 0.0 .. 1.0

    // The current playback position, according to `getData()` and
    // `audioCallback()`. Units: samples
    static int m_pos;

    // Pointer to function for offset calculate. Differs between different
    // audio formats.
    static int (*m_calculateOffset)(int sample, int channel);

    // Pointer to function for writing data. Differs between different audio
    // formats.
    static void (*m_writeData)(uint8_t* ptr, double data);

    // Called by `audioCallback` to generate audio data.
    static double getData();

    // This is function is called repeatedly by SDL2 to send data to the audio
    // device.
    static void audioCallback(
        void* userdata,
        uint8_t* stream,
        int len
    );
};
