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
	int Update(std::vector<UserCMD>* p_userCMD, float p_dt);
	void Draw();
	void Initialize();
};

