//Working just coment back

#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include <vector>
#include <map>
#include <string>
#include "Struct_AudioHolder.h"


class AudioManager
{
private:
	FMOD::System *m_system;
	FMOD_RESULT m_result;
	unsigned int m_version;
	FMOD_SPEAKERMODE m_speakerMode;
	FMOD_CAPS m_caps;
	char m_name[256];
	std::map<std::string,AudioHolder> m_sounds;
public:
	AudioManager(void);
	~AudioManager(void);
	void Initialize();
	void CreateSound(std::string p_fileName);
	void PlaySpecificSound(std::string p_soundToPlay, bool p_loop);
	void StopSpecificSound(std::string p_soundToStop);
	void CleanUpCrew();
};

