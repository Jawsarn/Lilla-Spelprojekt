#pragma once
#include "menuscreen.h"
#define MAX_VOLUME 10
#define VOLUME_CHANGE 0.1

class OptionsScreen :
	public MenuScreen
{
private:
	/////////////VARIABLES/////////////
	float m_volume;
	DirectX::XMFLOAT2 m_resolution;
	unsigned int m_hudHandle;
private:
	////////////FUNCTIONS/////////////
	void AlterVolume(UserCMD& p_userCMD);
public:
	OptionsScreen(void);
	OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle);
	~OptionsScreen(void);
	int Update(std::vector<UserCMD>* userCMD, float p_dt);
	void Draw();
	void Initialize();
};

