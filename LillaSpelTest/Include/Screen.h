#pragma once
#include "UserCMD.h"
#include <vector>

enum ApplicationState{GAME_SETUP_SCREEN, PAUSE_SCREEN, GAME_SCREEN, JOIN_GAME_SCREEN,OPTIONS_SCREEN, MAIN_MENU_SCREEN, SHUT_DOWN};

class Screen
{
protected:
	
public:
	Screen(void);
	~Screen(void);

	int virtual Update(float p_dt, std::vector<UserCMD> p_userCMDS);
	void virtual Draw();
};

