#include "Audio.hpp"

namespace AudioHandler {
	AudioHandler::AudioHandler() {
		channels[0].waveforms = Waveform::SQUARE;
		channels[0].frequency = 1400;
		channels[0].attack_ms = 5;
		channels[0].decay_ms = 100;
		channels[0].sustain = 0;
		channels[0].release_ms = 5;
	}

	void AudioHandler::set_volume(uint32_t volume) {
		for (uint8_t i = 0; i < 8; i++) {
			set_volume(i, volume);
		}
	}

	void AudioHandler::set_volume(uint8_t channel, uint32_t volume) {
		//blit::channels[channel].volume = volume;
	}

	void AudioHandler::load(uint8_t channel, const uint8_t mp3_data[], const uint32_t mp3_size) {
		if (channel > 7) {
			return;
		}
		/*blit::File::add_buffer_file(filenames[channel], mp3_data, mp3_size);
		load(channel, channel);*/
	}

	void AudioHandler::load(uint8_t target_channel, uint8_t source_channel) {
		//mp3_channels[target_channel].load(filenames[source_channel]);
	}

	void AudioHandler::play(uint8_t channel, uint8_t flags) {
		if (channel <= 2) {
			channels[0].trigger_attack();
			t = 0.1;
		}
		/*mp3_channels[channel].pause();
		mp3_channels[channel].restart();
		mp3_channels[channel].play(channel, flags);*/

		// TODO: make a boop sound instead
	}

	bool AudioHandler::is_playing(uint8_t channel) {
		// Requires player to press A for splash screen to end
		return true;
		//return blit::channels[channel].adsr_phase != blit::ADSRPhase::OFF;
	}

	void AudioHandler::update(float dt) {
		if (t > 0.0f) {
			t -= dt;
			if (t <= 0.0f) {
				channels[0].trigger_release();
			}
		}
		/*for (uint8_t i = 0; i < 8; i++) {
			mp3_channels[i].update();
		}*/
	}
}