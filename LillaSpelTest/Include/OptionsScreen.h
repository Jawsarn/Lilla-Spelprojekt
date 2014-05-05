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
	bool m_fullScreen;
private:
	////////////FUNCTIONS/////////////
	void AlterVolume(UserCMD& p_userCMD);
public:
	OptionsScreen(void);
	OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle);
	~OptionsScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
};

