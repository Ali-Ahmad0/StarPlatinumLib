#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>

struct AudioData 
{
	SDL_AudioSpec audioSpec;
	uint8_t* buffer = nullptr;
	uint32_t length = 0;
};

class AudioPlayer
{
public:
	static void Init();
	static void LoadAudio(const char* name, const char* path);
	static void PlayAudio(const char* name);
	static void Cleanup();

private:
	static SDL_AudioDeviceID s_audioDeviceId;
	static std::unordered_map<std::string, AudioData> s_audios;
};