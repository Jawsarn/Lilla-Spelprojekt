//Working just coment back

#include "AudioManager.h"

AudioManager::AudioManager(void)
{

}

AudioManager::~AudioManager(void)
{
}

void AudioManager::Initialize()
{
	if (FMOD::System_Create(&m_system) != FMOD_OK)
	{
		return;
	}

	int t_driverCount = 0;
	m_system->getNumDrivers(&t_driverCount);

	if (t_driverCount == 0)
	{
		return;
	}

	m_system->init(36,FMOD_INIT_NORMAL,NULL);
}

void AudioManager::CreateSound(std::string p_fileName)
{
	FMOD::Sound* sound;	
	if (m_sounds[p_fileName].sound == nullptr)
	{
		m_result = m_system->createSound(p_fileName.c_str(), FMOD_DEFAULT,0, &sound);

		m_sounds[p_fileName].sound=sound;
	}

}

void AudioManager::PlaySpecificSound(std::string p_soundToPlay, bool p_loop)
{
	if (m_sounds[p_soundToPlay].sound != nullptr)
	{
		if (!p_loop)
		{	
			m_sounds[p_soundToPlay].sound->setMode(FMOD_LOOP_OFF);	
		}
		else
		{
			m_sounds[p_soundToPlay].sound->setMode(FMOD_LOOP_NORMAL);
			m_sounds[p_soundToPlay].sound->setLoopCount(-1);
		}

		bool t_isPlaying;
		m_sounds[p_soundToPlay].channel->isPlaying(&t_isPlaying);
		if (t_isPlaying)
		{
			//m_soundChannels[t_index]->stop();
		}	
		m_result = m_system->playSound(FMOD_CHANNEL_FREE,m_sounds[p_soundToPlay].sound,false,&m_sounds[p_soundToPlay].channel);
	}
	else
	{
		CreateSound(p_soundToPlay);
		if (m_result == FMOD_OK)
		{
			PlaySpecificSound(p_soundToPlay,p_loop);
		}
	}
}

void AudioManager::CleanUpCrew()
{

}