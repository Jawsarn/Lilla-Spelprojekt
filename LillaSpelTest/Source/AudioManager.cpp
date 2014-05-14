//Working just coment back

#include "AudioManager.h"
AudioManager* AudioManager::m_Singleton = nullptr;

AudioManager::AudioManager(void)
{

}

AudioManager::~AudioManager(void)
{
}

AudioManager* AudioManager::GetInstance()
{
	if(m_Singleton == nullptr)
	{
		m_Singleton = new AudioManager();
		m_Singleton->Initialize();
	}
	return m_Singleton;
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
	
	m_masterVolume=1;
}

void AudioManager::Update()
{
	m_system->update();
}


void AudioManager::CreateSound(std::string p_fileName)
{
	FMOD::Sound* sound;	
	if (m_sounds[p_fileName].sound == nullptr)
	{
		m_result = m_system->createSound(p_fileName.c_str(), FMOD_DEFAULT,0, &sound);

		m_sounds[p_fileName].sound=sound;
		m_sounds[p_fileName].volume = 1;
	}

}

void AudioManager::PlaySpecificSound(std::string p_soundToPlay, bool p_loop, bool p_multipleSoundsOfSameSource)
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

		if (p_multipleSoundsOfSameSource)
		{
			m_result = m_system->playSound(FMOD_CHANNEL_FREE,m_sounds[p_soundToPlay].sound,false,&m_sounds[p_soundToPlay].channel);
		}
		else
		{
			bool t_isPlaying;
			m_sounds[p_soundToPlay].channel->isPlaying(&t_isPlaying);
			if (!t_isPlaying)
			{
				//m_soundChannels[t_index]->stop();
				m_result = m_system->playSound(FMOD_CHANNEL_FREE,m_sounds[p_soundToPlay].sound,false,&m_sounds[p_soundToPlay].channel);
			}
		}	
		SetSpecificSoundVolume(p_soundToPlay,1);
	}
	else
	{
		CreateSound(p_soundToPlay);
		if (m_result == FMOD_OK)
		{
			PlaySpecificSound(p_soundToPlay,p_loop,p_multipleSoundsOfSameSource);
		}
	}
}

void AudioManager::StopSpecificSound(std::string p_soundToStop)
{
	m_sounds[p_soundToStop].channel->stop();
}

void AudioManager::SetSpecificSoundVolume(std::string p_soundToIncrease, float p_volumeBetween0and1)
{
	m_result = m_sounds[p_soundToIncrease].channel->setPaused(true);
	m_result = m_sounds[p_soundToIncrease].channel->setVolume(m_masterVolume*p_volumeBetween0and1);
	m_result = m_sounds[p_soundToIncrease].channel->setPaused(false);
	m_sounds[p_soundToIncrease].volume=p_volumeBetween0and1;
}

void AudioManager::SetMasterVolume(float p_volumeBetween0and1)
{
	for (std::map<std::string, AudioHolder>::iterator it = m_sounds.begin(); it != m_sounds.end(); it++)
	{
		it->second.channel->setPaused(true);
		if (true)
		{
			m_result = it->second.channel->setVolume(p_volumeBetween0and1 * it->second.volume);
		}
	
		
		it->second.channel->setPaused(false);
	}
	m_masterVolume = p_volumeBetween0and1;
}
void AudioManager::PitchSpecificSound(std::string p_soundToPitch, float p_pitchValue)
{
	m_sounds[p_soundToPitch].channel->setFrequency(p_pitchValue);
}

void AudioManager::RemoveSpecificSound(std::string p_soundToRemove)
{
	m_sounds[p_soundToRemove].sound->release();
	m_sounds.erase(p_soundToRemove);
	
}

void AudioManager::CleanUpCrew()
{

}

float AudioManager::GetMasterVolume()
{
	return m_masterVolume;
}