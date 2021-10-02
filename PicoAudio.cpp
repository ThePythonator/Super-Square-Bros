#include "Audio.hpp"

namespace AudioHandler {
	AudioHandler::AudioHandler() {
	}

	void AudioHandler::init() {
		// Coin A / Select
		blit::channels[0].waveforms = blit::Waveform::SQUARE;
		blit::channels[0].frequency = 880;
		blit::channels[0].attack_ms = 100;
		blit::channels[0].decay_ms = 100;
		blit::channels[0].sustain = 0;
		blit::channels[0].release_ms = 0;

		// Jump
		blit::channels[1].waveforms = blit::Waveform::SQUARE;
		blit::channels[1].frequency = 440;
		blit::channels[1].attack_ms = 150;
		blit::channels[1].decay_ms = 1;
		blit::channels[1].sustain = 0;
		blit::channels[1].release_ms = 0;

		// Coin B
		blit::channels[2].waveforms = blit::Waveform::SQUARE;
		blit::channels[2].frequency = 932;
		blit::channels[2].attack_ms = 100;
		blit::channels[2].decay_ms = 100;
		blit::channels[2].sustain = 0;
		blit::channels[2].release_ms = 0;

		// Enemy Death
		blit::channels[3].waveforms = blit::Waveform::SQUARE;
		blit::channels[3].frequency = 300;
		blit::channels[3].attack_ms = 100;
		blit::channels[3].decay_ms = 100;
		blit::channels[3].sustain = 0;
		blit::channels[3].release_ms = 0;

		// Enemy Injured
		blit::channels[4].waveforms = blit::Waveform::SQUARE;
		blit::channels[4].frequency = 300;
		blit::channels[4].attack_ms = 100;
		blit::channels[4].decay_ms = 100;
		blit::channels[4].sustain = 0;
		blit::channels[4].release_ms = 0;

		// Player Death
		blit::channels[5].waveforms = blit::Waveform::SQUARE;
		blit::channels[5].frequency = 500;
		blit::channels[5].attack_ms = 200;
		blit::channels[5].decay_ms = 20;
		blit::channels[5].sustain = 0;
		blit::channels[5].release_ms = 0;

		// Player Injured
		blit::channels[6].waveforms = blit::Waveform::SQUARE;
		blit::channels[6].frequency = 450;
		blit::channels[6].attack_ms = 150;
		blit::channels[6].decay_ms = 20;
		blit::channels[6].sustain = 0;
		blit::channels[6].release_ms = 0;

		// Pico Tune!
		blit::channels[7].waveforms = blit::Waveform::SQUARE;
		blit::channels[7].frequency = 450;
		blit::channels[7].attack_ms = 125; //500ish ms per beat
		blit::channels[7].decay_ms = 20;
		blit::channels[7].sustain = 0;
		blit::channels[7].release_ms = 0;
	}

	void AudioHandler::set_volume(uint32_t volume) {
		(void)volume;
	}

	void AudioHandler::set_volume(uint8_t channel, uint32_t volume) {
		(void)channel;
		(void)volume;
	}

	void AudioHandler::load(uint8_t channel, const uint8_t mp3_data[], const uint32_t mp3_size) {
		(void)channel;
		(void)mp3_data;
		(void)mp3_size;
	}

	void AudioHandler::load(uint8_t target_channel, uint8_t source_channel) {
		(void)target_channel;
		(void)source_channel;
	}

	void AudioHandler::play(uint8_t channel, uint8_t flags) {
		if (channel <= 6) {
			blit::channels[channel].trigger_attack();
		}
		else if (channel == 7) {
			// Play a tune!
			play_tune = true;
			note = 0;
			t = 0.0f;
		}
	}

	bool AudioHandler::is_playing(uint8_t channel) {
		// Requires player to press A for splash screen to end
		return true;
	}

	void AudioHandler::update(float dt) {
		blit::channels[1].frequency = 600 + (blit::channels[1].adsr >> 16);

		blit::channels[0].frequency = blit::channels[0].adsr_phase == blit::ADSRPhase::ATTACK ? 880 : 1318;
		blit::channels[2].frequency = blit::channels[2].adsr_phase == blit::ADSRPhase::ATTACK ? 932 : 1396;

		//blit::channels[3].frequency = 300 - (blit::channels[3].adsr >> 16);
		//blit::channels[4].frequency = 300 - (blit::channels[1].adsr >> 16);

		blit::channels[5].frequency = 500 - (blit::channels[5].adsr >> 16);

		blit::channels[6].frequency = 450 - (blit::channels[6].adsr >> 16) / 2;

		if (play_tune) {
			t += dt;
			if (t > 0.150f) {
				t = 0.0f;
				if (tune[note] != 0) {
					blit::channels[7].frequency = std::pow(2, (tune[note] - 69) / 12.0f) * 440;
					blit::channels[7].trigger_attack();
				}
				note++;
				if (note == 24) {
					play_tune = false;
				}
			}
		}
	}
}