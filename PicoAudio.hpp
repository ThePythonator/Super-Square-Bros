#pragma once

#include "audio/mp3-stream.hpp"

namespace AudioHandler {
	class AudioHandler {
	public:
		AudioHandler();

		void set_volume(uint32_t = 0xffff);
		void set_volume(uint8_t, uint32_t);
		
		void load(uint8_t, const uint8_t[], const uint32_t);
		void load(uint8_t, uint8_t);
		void play(uint8_t, uint8_t = 0);
		bool is_playing(uint8_t);
		void update();
	};
}