#pragma once
#include "menuscreen.h"
#define MAX_VOLUME 1
#define VOLUME_CHANGE 0.05

class OptionsScreen :
	public MenuScreen
{
private:
	/////////////VARIABLES/////////////
	float m_volume;
	DirectX::XMFLOAT2 m_resolution;
	unsigned int m_hudHandle;
	unsigned int m_controllerMappingHandle;
	bool m_fullScreen;
	unsigned int m_volumeSlideHandle;
private:
	////////////FUNCTIONS/////////////
	void AlterVolume(UserCMD& p_userCMD);
public:
	OptionsScreen(void);
	OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager);
	~OptionsScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
};

