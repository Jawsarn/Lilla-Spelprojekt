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
void GameScreen::Initialize()
{

}


int GameScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDS) 
{
	////////////////JOHNS TEST MÖS!!! kommentera bort så fungerar allt som en neger på en bomullsfarm
	for (int i = 0; i < 1; i++)			//i<1 for test purposes. Make sure to change later
	{
		bool collision = false;
		PlayerWall* t_newWall = m_players[i]->GetLastPlacedWall(); //

		//fixes the position, direction, sets up world matrix, drops wall, etc.
		if(m_players[i]->ProperUpdatePosition(p_dt, p_userCMDS->at(i)) == 1)
		{
			PlayerWall* t_newWall = m_players[i]->GetLastPlacedWall();
			t_newWall->m_wallIndex = m_graphicHandle->CreateWall(0, t_newWall->GetWorldMatrix(), i);			//PUBLIC VARIALBE!!! MAKE FIX BEFORE JAWS DISCOVERS!!!t_newWall->GetWorldMatrix()
		}
		//gets the world matrix
		m_graphicHandle->JohnSetCamera(m_players[i]->GetWorldMatrix(), i);
		MapNode* t_currMapNode = m_players[i]->GetCurrentMapNode();    //för att slippa getta flera gånger i denna forsats

		//check collision for player i against all wall objects in his mapnode
		vector<StaticObj*>* m_wallsToCheck = &t_currMapNode->m_staticObjs;			///Create list of all the static objs in the currmapNode
		if(m_collisionManager->PlayerVsObj(m_players[i]->GetCollisionBox(), m_wallsToCheck)!=1)    ///PLAYER VS STATIC Collisionmanager checks for hit and returns int representing the object you collided with
		{
			p_userCMDS->at(i).controller.Vibrate(64000,64000);										///PLAYER VS STATIC
			collision = true;
		}
		///END OF PLAYER VS STATIC

		//check collision for play i against all playerwall objects in his mapnode
		vector<PlayerWall*>* t_playerWallsToCheck = &t_currMapNode->m_playerWalls;					//Create a new list of the PlayerWalls in currmapnode
		if (t_playerWallsToCheck->size()>0)
		{
			if(t_newWall == t_currMapNode->m_playerWalls.at(t_currMapNode->m_playerWalls.size()-1))		///PLAYER VS PLAYERWALL  Checks if the wall you placed this frame is in the list and removes it
			{
				t_playerWallsToCheck->pop_back();														///PLAYER VS PLAYERWALL
			}
		}
		if(m_collisionManager->PlayerVsPlayerWall(m_players[i]->GetCollisionBox(), t_playerWallsToCheck))		///PLAYER VS PLAYERWALL Collisionmanager returns true if hit
		{
			p_userCMDS->at(i).controller.Vibrate(64000,64000);											///PLAYER VS PLAYERWALL
			collision = true;																			///PLAYER VS PLAYERWALL
		}
		if (!collision)
			p_userCMDS->at(i).controller.Vibrate(0,0);


	}
	////////////////JOHNS TEST MÖS ENDS!!
	/*for (int i = 0; i < 4; i++)
	{	
	m_graphicHandle->UpdateCamera(i,0,0,0,p_userCMDS->at(i).Joystick.y*p_dt,p_userCMDS->at(i).Joystick.x*p_dt); freelokshits
	if (p_userCMDS->at(i).leftTriggerPressed)
	{
	m_graphicHandle->UpdateCamera(i,100*p_dt,0,0,0,0);
	}
	if (p_userCMDS->at(i).rightTriggerPressed)
	{
	m_graphicHandle->UpdateCamera(i,-100*p_dt,0,0,0,0);
	}
	}*/
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