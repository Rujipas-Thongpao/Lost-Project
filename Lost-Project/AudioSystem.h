#pragma once
#include <string>
#include <unordered_map>
#include <irrKlang.h>

struct SoundConfig {
	bool isLoop     = false;
	float volume    = 1.0f;  // 0.0 - 1.0
};

class AudioSystem {
public:
	void Init();
	void Shutdown();

	void Load(const std::string& key, const std::string& filepath);

	irrklang::ISound* Play(const std::string& key, const SoundConfig& cfg = {});

	void StopAll();

	void SetMasterVolume(float v);
	float GetMasterVolume() const;

private:
	irrklang::ISoundEngine* engine = nullptr;
	std::unordered_map<std::string, irrklang::ISoundSource*> sources;
};
