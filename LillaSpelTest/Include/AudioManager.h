//Working just coment back

#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include <vector>
#include <map>
#include <string>
#include "Struct_AudioHolder.h"

enum AudioPlayFunctions {AUDIO_STOP_AND_PLAY, AUDIO_ONLY_PLAY_ONE, AUDIO_PLAY_MULTIPLE};
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
	static AudioManager* m_Singleton;
	float m_masterVolume;
	FMOD::Channel* m_engineChannels[4];

	AudioManager(void);
	~AudioManager(void);
	void Initialize();
public:
	AudioManager* GetInstance();
	//apparently the system needs to update every frame
	void Update();
	void CreateSound(std::string p_fileName);
	// the sound name is the files name: if the sound hasn't been created yet it will be created, if that fails the sound wont play
	void PlaySpecificSound(std::string p_soundToPlay, bool p_loop,AudioPlayFunctions p_multipleSoundsOfSameSource);
	// the sound name is the files name
	void StopSpecificSound(std::string p_soundToStop);
	// the sound name is the files name
	void SetSpecificSoundVolume(std::string p_soundToIncrease, float p_volumeBetween0and1);
	void SetMasterVolume(float p_volumeBetween0and1);
	// the sound name is the files name:the pitch value is normally 44100, higher than that makes the sound go faster and vice versa
	void PitchSpecificSound(std::string p_soundToPitch, float p_pitchValue);
	void CleanUpCrew();
	// the sound name is the files name. this is for removing sounds that wont get played for quite a while, like in game main soundtrack
	void RemoveSpecificSound(std::string p_soundToRemove);

	float GetMasterVolume();
};

