#include <SDL2/SDL.h>  

class Beeper
{
public:
    inline static SDL_AudioSpec m_obtainedSpec{};
    inline static SDL_AudioDeviceID m_audioDevice{0};
    inline static double m_frequency{0}; // Units: Hz
    inline static double m_volume{1.0}; // Range: 0.0 .. 1.0
    inline static int m_pos{0};
    
    static void open() {
		SDL_AudioSpec desiredSpec;
		SDL_zero(desiredSpec);
		desiredSpec.freq = 44100;
		desiredSpec.format = AUDIO_S16;
		desiredSpec.samples = 512;
		desiredSpec.channels = 1;
		desiredSpec.callback = Beeper::audioCallback;
		m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &m_obtainedSpec, 0);
	}

    static void close() { SDL_CloseAudioDevice(Beeper::m_audioDevice); }
    static void setFrequency(double frequency) { m_frequency = frequency; }
    static void setVolume(double volume) { m_volume = volume; }
    static void play() { SDL_PauseAudioDevice(Beeper::m_audioDevice, 0); }
    static void stop() { SDL_PauseAudioDevice(Beeper::m_audioDevice, 1); }
    
    static int m_calculateOffset(int sample, int channel) {
		return (sample * Beeper::m_obtainedSpec.channels + channel) * 
			m_obtainedSpec.format == AUDIO_S16 ? sizeof(short) : sizeof(float);
	}
    
    static void m_writeData(float* ptr, double data) {
		*ptr = m_obtainedSpec.format == AUDIO_S16 ? data * (INT16_MAX - INT16_MIN) / 2.0 : data;
	}

    static double getData() {
		if (m_frequency) {
			double sampleRate = (double)(m_obtainedSpec.freq);
			double period = sampleRate / m_frequency;
			if (m_pos % (int)period == 0) { m_pos = 0; }
			double pos = m_pos;
			double angular_freq = (1.0 / period) * 2.0 * M_PI;
			double amplitude = m_volume;
			double sin_ = sin(pos * angular_freq) > 0 ? 1 : 0;

			return sin_ * amplitude;
		}
		return 0;
	}

    static void audioCallback(void* userdata, uint8_t* stream, int len) {
		(void)userdata;
		(void)len;

		for (int sample = 0; sample < m_obtainedSpec.samples; ++sample) {
			double data = getData();
			m_pos++;

			for (int channel = 0; channel < m_obtainedSpec.channels; ++channel) {
				m_writeData((float*)(stream + m_calculateOffset(sample, channel)), data);
			}
		}
	}
};
