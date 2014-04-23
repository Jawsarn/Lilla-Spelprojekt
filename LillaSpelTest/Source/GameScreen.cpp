#include "GameScreen.h"


GameScreen::GameScreen(void)
{
}

GameScreen::GameScreen(std::string p_mapName, int p_numberOfPlayers )
{
	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		m_players.push_back(new Player());  
	}
}

GameScreen::~GameScreen(void)
{
}


int GameScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDS, GraphicHandle* m_GraphicHandle)//the graphichandle pointer shouldnt be here, put it in the screen class instead
{
	////Free moving////
	for (int i = 0; i < 4; i++)
	{	
		m_GraphicHandle->UpdateCamera(i,0,0,0,p_userCMDS->at(i).Joystick.y*p_dt,p_userCMDS->at(i).Joystick.x*p_dt);
		if (p_userCMDS->at(i).leftTriggerPressed)
		{
			m_GraphicHandle->UpdateCamera(i,100*p_dt,0,0,0,0);
		}
		if (p_userCMDS->at(i).rightTriggerPressed)
		{
			m_GraphicHandle->UpdateCamera(i,-100*p_dt,0,0,0,0);
		}
	}
	return 1;
}

void GameScreen::Draw()
{}