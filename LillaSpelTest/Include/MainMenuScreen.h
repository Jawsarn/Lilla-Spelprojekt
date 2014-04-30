#pragma once
#include "menuscreen.h"
class MainMenuScreen :
	public MenuScreen
{
private:
	unsigned int m_hudHandle;
public:
	MainMenuScreen(void);
	MainMenuScreen(GraphicHandle* p_graphicsHandle);
	~MainMenuScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
};

