#pragma once
#include "menuscreen.h"
class PauseScreen :
	public MenuScreen
{
private:
	unsigned int m_hudHandle;
	int m_whoPaused;
public:
	PauseScreen(void);
	PauseScreen(GameInfo* p_gameInfo ,GraphicHandle* p_graphicHandle, AudioManager* p_audioManager);
	~PauseScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize(int p_whoPaused);
	void Initialize();
};

