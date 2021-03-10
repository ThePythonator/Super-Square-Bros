#include "Audio.hpp"

// Todo: rework to pick a free channel rather than always using same one.

namespace AudioHandler {
	AudioHandler::AudioHandler() { }

	void AudioHandler::set_volume(uint32_t volume) {
		for (uint8_t i = 0; i < 8; i++) {
			set_volume(i, volume);
		}
	}

	void AudioHandler::set_volume(uint8_t channel, uint32_t volume) {
		blit::channels[channel].volume = volume;
	}

	void AudioHandler::load(uint8_t channel, const uint8_t mp3_data[], const uint32_t mp3_size) {
		if (channel > 7) {
			return;
		}
		blit::File::add_buffer_file(filenames[channel], mp3_data, mp3_size);
		load(channel, channel);
	}

	void AudioHandler::load(uint8_t target_channel, uint8_t source_channel) {
		mp3_channels[target_channel].load(filenames[source_channel]);
	}

	void AudioHandler::play(uint8_t channel, uint8_t flags) {
		mp3_channels[channel].pause();
		mp3_channels[channel].restart();
		mp3_channels[channel].play(channel, flags);
	}

	void AudioHandler::update() {
		for (uint8_t i = 0; i < 8; i++) {
			mp3_channels[i].update();
		}
	}
}