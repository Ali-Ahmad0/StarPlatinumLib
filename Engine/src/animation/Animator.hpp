#pragma once
#include "Animation.hpp"

template <typename T>
class Animator 
{
public:
	void AddTrack(const char* trackName)
	{
		std::vector<Animation<T>> animations{};
		m_tracks[trackName] = animations;
	}

	void AddAnimation(const char* trackName, Animation<T> animation)
	{
		// Add the animation to track if exists
		auto it = m_tracks.find(trackName);
		if (it == m_tracks.end())
		{
			fprintf(stderr, "[ERROR]: Animation track %s does not exist\n", trackName);
			return;
		}
		it->second.push_back(animation);
	}
	
	void PlayTrack(const char* trackName, double delta) 
	{
		auto it = m_tracks.find(trackName);
		if (it == m_tracks.end())
		{
			fprintf(stderr, "[ERROR]: Animation track %s does not exist\n", trackName);
			return;
		}

		for (Animation<T>& animation : it->second) 
		{
			animation.Play(delta);
		}
	}

	void ResetTrack(const char* trackName) 
	{
		auto it = m_tracks.find(trackName);
		if (it == m_tracks.end())
		{
			fprintf(stderr, "[ERROR]: Animation track %s does not exist\n", trackName);
			return;
		}

		for (Animation<T>& animation : it->second) 
		{
			animation.Reset();
		}
	}

	bool IsFinished(const char* trackName) 
	{
		auto it = m_tracks.find(trackName);
		if (it == m_tracks.end())
		{
			fprintf(stderr, "[ERROR]: Animation track %s does not exist\n", trackName);
			return false;
		}

		for (Animation<T>& animation : it->second)
		{
			if (!animation.IsFinished()) 
			{
				return false;
			}
		}

		// Return true if all animations in track are finished
		return true;
	}

private:
	std::unordered_map<const char*, std::vector<Animation<T>>> m_tracks{};
};