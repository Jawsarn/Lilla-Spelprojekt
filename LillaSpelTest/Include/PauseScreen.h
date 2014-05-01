#pragma once
#include "menuscreen.h"
class PauseScreen :
	public MenuScreen
{
public:
	PauseScreen(void);
	~PauseScreen(void);
	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
};

