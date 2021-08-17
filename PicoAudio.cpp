#include "PicoAudio.hpp"

// Todo: rework to pick a free channel rather than always using same one.

namespace AudioHandler {
	AudioHandler::AudioHandler() { }

	void AudioHandler::set_volume(uint32_t volume) {
		(void)volume;
	}

	void AudioHandler::set_volume(uint8_t channel, uint32_t volume) {
		(void)channel;
		(void)volume;
	}

	void AudioHandler::load(uint8_t channel, const uint8_t mp3_data[], const uint32_t mp3_size) {
		(void)channel;
		(void *)mp3_data;
		(void)mp3_size;
	}

	void AudioHandler::load(uint8_t target_channel, uint8_t source_channel) {
		(void)target_channel;
		(void)source_channel;
	}

	void AudioHandler::play(uint8_t channel, uint8_t flags) {
		(void)channel;
		(void)flags;
	}

	bool AudioHandler::is_playing(uint8_t channel) {
		(void)channel;
		return true; // Make splash wait for button press
	}

	void AudioHandler::update() {
	}
}