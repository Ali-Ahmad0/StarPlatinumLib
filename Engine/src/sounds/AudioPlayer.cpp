#include "AudioPlayer.hpp"
#include <stdio.h>

SDL_AudioDeviceID AudioPlayer::s_audioDeviceId = 0;
std::unordered_map<std::string, AudioData> AudioPlayer::s_audios{};

void AudioPlayer::Init() 
{
	// Create a default audio spec to load audio device
	SDL_AudioSpec defaultSpec;
	SDL_memset(&defaultSpec, 0, sizeof(defaultSpec));
	defaultSpec.freq = 44100;
	defaultSpec.format = AUDIO_F32;
	defaultSpec.channels = 2;
	defaultSpec.samples = 4096;

	s_audioDeviceId = SDL_OpenAudioDevice(NULL, 0, &defaultSpec, NULL, 0);
	if (s_audioDeviceId == 0) 
	{
		fprintf(stderr, "[ERROR]: Error loading audio device: %s\n", SDL_GetError());
		return;
	}

	// Start audio
	SDL_PauseAudioDevice(s_audioDeviceId, 0);
}

void AudioPlayer::LoadAudio(const char* name, const char* path)
{
	AudioData data;
	if (!SDL_LoadWAV(path, &data.audioSpec, &data.buffer, &data.length)) 
	{
		fprintf(stderr, "[ERROR]: Error loading audio file - %s\n", path);
		return;
	}

	s_audios[name] = data;
}

void AudioPlayer::PlayAudio(const char* name)
{
	auto it = s_audios.find(name);
	if (it == s_audios.end()) 
	{
		fprintf(stderr, "[ERROR]: Audio %s does not exist, make sure to load it first\n", name);
		return;
	}

	// Play the audio
	SDL_QueueAudio(s_audioDeviceId, it->second.buffer, it->second.length);
}

void AudioPlayer::Cleanup() 
{
	// Close audio data
	for (auto& pair : s_audios) 
	{
		AudioData* data = &pair.second;
		SDL_FreeWAV(data->buffer);
	}
	s_audios.clear();
	
	// Close audio device
	if (s_audioDeviceId != 0) 
	{
		SDL_CloseAudioDevice(s_audioDeviceId);
		s_audioDeviceId = 0;
	}

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}