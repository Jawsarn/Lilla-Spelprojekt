#pragma once
#include "UserCMD.h"
#include "GraphicHandle.h"
#include "AudioManager.h"
#include <vector>

enum ApplicationState{GAME_SETUP_SCREEN,  GAME_SCREEN,PAUSE_SCREEN, JOIN_GAME_SCREEN,OPTIONS_SCREEN, MAIN_MENU_SCREEN, GOAL_SCREEN,SHUT_DOWN, DEBUGG};

class Screen
{
protected:
	AudioManager* m_audioManager;
	GraphicHandle* m_graphicHandle;
public:
	Screen(void);
	
	Screen(GraphicHandle* p_graphicHandle);
	Screen(GraphicHandle* p_graphicHandle, AudioManager* p_audioManager);
	~Screen(void);

	int virtual Update(float p_dt, std::vector<UserCMD>* p_userCMDS);
	void virtual Initialize() = 0;
	void virtual Draw();
};

