#pragma once

#ifdef PICO_BUILD
#include <cmath>
#include "audio/audio.hpp"
#else
#include "audio/mp3-stream.hpp"
#endif // !PICO_BUILD


namespace AudioHandler {
	class AudioHandler {
	public:
		AudioHandler();

		void init();

		void set_volume(uint32_t = 0xffff);
		void set_volume(uint8_t, uint32_t);
		
		void load(uint8_t, const uint8_t[], const uint32_t);
		void load(uint8_t, uint8_t);
		void play(uint8_t, uint8_t = 0);
		bool is_playing(uint8_t);
		void update(float dt);

	protected:
#ifdef PICO_BUILD
		uint8_t tune[24] = {
			75, 80, 84, 0, 0, 0, 0, 0,
			77, 82, 85, 0, 0, 0, 0, 0,
			87, 87, 87, 92, 0, 0, 0, 0,
		};
		uint8_t note = 0;
		bool play_tune = true;
		float t = 0.0f;
		bool sfx_mute = false;
		bool music_mute = false;
#else
		blit::MP3Stream mp3_channels[8];
		const char* filenames[8] = {
			"temp0.mp3", "temp1.mp3", "temp2.mp3", "temp3.mp3", "temp4.mp3", "temp5.mp3", "temp6.mp3", "temp7.mp3"
		};
#endif // !PICO_BUILD
	};
}
