#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <cmath>

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;
uint16_t freq_;
bool toSample;
uint32_t v;

void tone(uint8_t _, uint16_t _freq)
{
    freq_ = _freq;
    toSample = true;
}

void noTone(uint8_t _)
{
    toSample = false;
}

void audio_callback(void *_beeper, Uint8 *_stream, int _length)
{
    Sint16 *stream = (Sint16*) _stream;
    int length = _length / 2;
    int i = 0;
	if (toSample)
		while (i < length) {
			stream[i++] = AMPLITUDE * std::sin(v * M_PI / FREQUENCY);
			v += freq_ * 2;
		}
	else
		while (i < length)
			stream[i++] = 0;
}

void init_beeper()
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec desiredSpec;
    desiredSpec.freq = FREQUENCY;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    SDL_AudioSpec obtainedSpec;
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);
    SDL_PauseAudio(0);
}
