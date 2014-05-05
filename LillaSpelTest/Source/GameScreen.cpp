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
	m_graphicHandle->SetAmountOfPlayers(p_numberOfPlayers);
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
	for (int i = 0; i < 2; i++)			//i<1 for test purposes. Make sure to change later
	{
		if(p_userCMDS->at(i).leftTriggerPressed)
			return PAUSE_SCREEN;
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
			//p_userCMDS->at(i).controller.Vibrate(64000,64000);										///PLAYER VS STATIC
			//collision = true;
		}
		///END OF PLAYER VS STATIC

		////PLAYER VS PLAYERWALL
		///check collision for play i against all playerwall objects in his mapnode
		//gets the walls for the current mapnode
		vector<PlayerWall*> t_playerWallsToCheck = t_currMapNode->m_playerWalls;
		//ensures that collision check is not made against the last wall placed by the player
		if (t_playerWallsToCheck.size()>0)
		{
			if(t_newWall == t_currMapNode->m_playerWalls.at(t_currMapNode->m_playerWalls.size()-1))		
			{
				t_playerWallsToCheck.pop_back();														///PLAYER VS PLAYERWALL
			}
		}
		///checks collision for the playet against all walls in the current mapnode
		//returns -1 if player hits a wall. Returns the number of spheres hit (used for boost calculation) otherwise
		int t_collisionResult = m_collisionManager->PlayerVsPlayerWall(m_players[i]->GetCollisionBox(), t_playerWallsToCheck);
		if(t_collisionResult == -1)	
		{
			p_userCMDS->at(i).controller.Vibrate(64000,64000);											
			collision = true;														
		}
		else if (t_collisionResult>0)
		{
			float t_boostPerWallPerUpdate = 1;
			int t_currentBoost = m_players[i]->GetPlayerBoost();
			m_players[i]->SetPlayerBoost(t_currentBoost+p_dt*t_collisionResult*t_boostPerWallPerUpdate);
			p_userCMDS->at(i).controller.Vibrate(10000,10000);
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






	return GAME_SCREEN;
}

void GameScreen::Draw()
{

}