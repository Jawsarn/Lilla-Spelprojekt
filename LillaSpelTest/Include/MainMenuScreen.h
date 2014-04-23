#pragma once
#include "menuscreen.h"
class MainMenuScreen :
	public MenuScreen
{
public:
	MainMenuScreen(void);
	~MainMenuScreen(void);
	int Update(std::vector<UserCMD>* p_userCMD, float p_dt);
	void Draw();
};

