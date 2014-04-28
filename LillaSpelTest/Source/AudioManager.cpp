//Working just coment back

//#include "AudioManager.h"
//
//AudioManager::AudioManager(void)
//{
//
//}
//
//AudioManager::~AudioManager(void)
//{
//}
//
//void AudioManager::Initialize()
//{
//	if (FMOD::System_Create(&m_system) != FMOD_OK)
//	{
//		return;
//	}
//
//	int t_driverCount = 0;
//	m_system->getNumDrivers(&t_driverCount);
//
//	if (t_driverCount == 0)
//	{
//		return;
//	}
//
//	m_system->init(36,FMOD_INIT_NORMAL,NULL);
//}
//
//void AudioManager::CreateSound(const char* p_fileName, int& o_soundHandle)
//{
//	FMOD::Sound* sound;	
//	m_result = m_system->createSound(p_fileName,FMOD_DEFAULT,0, &sound);
//	
//	m_allSounds.push_back(sound);
//	o_soundHandle = m_allSounds.size()-1;
//}
//
//void AudioManager::PlaySpecificSound(int p_soundToPlay, bool p_loop)
//{
//	if (!p_loop)
//	{
//		m_allSounds[p_soundToPlay]->setMode(FMOD_LOOP_OFF);
//	}
//	else
//	{
//		m_allSounds[p_soundToPlay]->setMode(FMOD_LOOP_NORMAL);
//		m_allSounds[p_soundToPlay]->setLoopCount(-1);
//	}
//	FMOD::Channel *channel;
//	m_result = m_system->playSound(FMOD_CHANNEL_FREE,m_allSounds[p_soundToPlay],false,&channel);
//}
//
//void AudioManager::CleanUpCrew()
//{
//	for (int i = 0; i < m_allSounds.size(); i++)
//	{
//		m_allSounds[i]->release();
//	}
//}