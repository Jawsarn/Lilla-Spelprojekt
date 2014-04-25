#include "GameScreen.h"


GameScreen::GameScreen(void)
{
}

GameScreen::GameScreen(std::string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle )
	:Screen(p_graphicHandle)
{
	m_mapLoader = new MapLoader();
	m_mapNodes = m_mapLoader->LoadMap(p_mapName);
	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		m_players.push_back(new Player(m_mapNodes->at(0),0.0f));
	}
}

GameScreen::~GameScreen(void)
{
}


int GameScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDS)
{
	////Free moving////
	for (int i = 0; i < 4; i++)
	{	
		m_graphicHandle->UpdateCamera(i,0,0,0,p_userCMDS->at(i).Joystick.y*p_dt,p_userCMDS->at(i).Joystick.x*p_dt);
		if (p_userCMDS->at(i).leftTriggerPressed)
		{
			m_graphicHandle->UpdateCamera(i,100*p_dt,0,0,0,0);
		}
		if (p_userCMDS->at(i).rightTriggerPressed)
		{
			m_graphicHandle->UpdateCamera(i,-100*p_dt,0,0,0,0);
		}
	}
	//for (int i = 0; i < 1; i++)			//i<1 for test purposes. Make sure to change later
	//{
	//	m_players[i]->ProperUpdatePosition(p_dt, p_userCMDS->at(i));
	//	m_graphicHandle->JohnSetCamera(m_players[i]->GetWorldMatrix(), i);
	//}

	




	return 1;
}

void GameScreen::Draw()
{

}