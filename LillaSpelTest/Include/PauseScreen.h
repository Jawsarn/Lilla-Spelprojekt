#pragma once
#include "menuscreen.h"
class PauseScreen :
	public MenuScreen
{
private:
	unsigned int m_hudHandle;
public:
	PauseScreen(void);
	PauseScreen(GraphicHandle* p_graphicHandle);
	~PauseScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize(int p_whoPaused);
	void Initialize();
};

