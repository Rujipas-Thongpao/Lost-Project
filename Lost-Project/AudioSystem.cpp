#include "AudioSystem.h"
#include <iostream>

void AudioSystem::Init() {
	engine = irrklang::createIrrKlangDevice();
	if (!engine)
		std::cerr << "[AudioSystem] Failed to create irrKlang device\n";
}

void AudioSystem::Shutdown() {
	if (engine) {
		engine->drop();
		engine = nullptr;
	}
}

void AudioSystem::Load(const std::string& key, const std::string& filepath) {
	if (!engine) return;
	irrklang::ISoundSource* src = engine->addSoundSourceFromFile(filepath.c_str());
	if (src)
		sources[key] = src;
	else
		std::cerr << "[AudioSystem] Failed to load: " << filepath << "\n";
}

irrklang::ISound* AudioSystem::Play(const std::string& key, const SoundConfig& cfg) {
	if (!engine) return nullptr;

	auto it = sources.find(key);
	irrklang::ISoundSource* src = (it != sources.end()) ? it->second : nullptr;

	irrklang::ISound* sound = nullptr;
	if (src)
		sound = engine->play2D(src, cfg.isLoop, false, true);
	else
		// fallback: let irrKlang locate the file by key as path
		sound = engine->play2D(key.c_str(), cfg.isLoop, false, true);

	if (sound) {
		sound->setVolume(cfg.volume);
		sound->setIsPaused(false);
	}
	return sound;
}

void AudioSystem::StopAll() {
	if (engine) engine->stopAllSounds();
}

void AudioSystem::SetMasterVolume(float v) {
	if (engine) engine->setSoundVolume(v);
}

float AudioSystem::GetMasterVolume() const {
	return engine ? engine->getSoundVolume() : 0.f;
}
