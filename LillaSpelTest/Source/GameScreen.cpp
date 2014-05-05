#include "GameScreen.h"


GameScreen::GameScreen(void)
{
}

GameScreen::GameScreen(int p_color[4], int p_whatVehicle[4], std::string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle )
	:Screen(p_graphicHandle)
{
	m_mapLoader = new MapLoader();
	m_mapNodes = m_mapLoader->LoadMap(p_mapName);
	vector<XMMATRIX> t_shipWorldMatrices;
	vector<UINT> t_colors;
	vector<UINT> t_whichVehicles;
	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		
		m_players.push_back(new Player(m_mapNodes->at(0),0.0f));
		t_shipWorldMatrices.push_back(m_players[i]->GetWorldMatrix());
		t_colors.push_back(p_color[i]);
		t_whichVehicles.push_back(p_whatVehicle[i]);
	}
	
	m_graphicHandle->SetAmountOfPlayers(p_numberOfPlayers);
	m_graphicHandle->SetColourAndVehicle(t_colors, t_whichVehicles);
	m_graphicHandle->CreateShipForGame(t_shipWorldMatrices);
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

	for (int i = 0; i < m_players.size(); i++)			//i<1 for test purposes. Make sure to change later
	{
		//if(p_userCMDS->at(i).leftTriggerPressed)
			//return PAUSE_SCREEN;
		bool collision = false;
		PlayerWall* t_newWall = m_players[i]->GetLastPlacedWall(); //

		//fixes the position, direction, sets up world matrix, drops wall, etc.
		//Update player and check if he placed a wall
		if(m_players[i]->ProperUpdatePosition(p_dt, p_userCMDS->at(i)) == 1)
		{
			
			PlayerWall* t_newWall = m_players[i]->GetLastPlacedWall();
			t_newWall->m_wallIndex = m_graphicHandle->CreateWall(0, t_newWall->GetWorldMatrix(), i);			//PUBLIC VARIALBE!!! MAKE FIX BEFORE JAWS DISCOVERS!!!t_newWall->GetWorldMatrix()
		}
		//gets the world matrix
		m_graphicHandle->JohnSetCamera(m_players[i]->GetWorldMatrix(), i);
		m_graphicHandle->UpdatePlayer(i, m_players[i]->GetWorldMatrix(), m_players[i]->GetCamMatrix());
		MapNode* t_currMapNode = m_players[i]->GetCurrentMapNode();    //för att slippa getta flera gånger i denna forsats

		//check collision for player i against all wall objects in his mapnode
		vector<StaticObj*>* m_wallsToCheck = &t_currMapNode->m_staticObjs;			///Create list of all the static objs in the currmapNode
		if(m_collisionManager->PlayerVsObj(m_players[i]->GetCollisionBox(), m_wallsToCheck)!=1)    ///PLAYER VS STATIC Collisionmanager checks for hit and returns int representing the object you collided with
		{
			////////////////INSERT PLAYER VS SATIC OBJECT CODE//////////////////
			//p_userCMDS->at(i).controller.Vibrate(64000,64000);										
			//collision = true;
		}


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
			//Collided with playerwall
			///////////////////////INSERT PLAYER VS PLAYERWALL CODE///////////////////
			p_userCMDS->at(i).controller.Vibrate(64000,64000);											
			collision = true;														
		}
		else if (t_collisionResult>0)
		{
			//went close to wall and got boost
			float t_boostPerWallPerUpdate = 10;
			int t_currentBoost = m_players[i]->GetPlayerBoost();
			m_players[i]->SetPlayerBoost(t_currentBoost+p_dt*t_collisionResult*t_boostPerWallPerUpdate);
			p_userCMDS->at(i).controller.Vibrate(10000,10000);
		}
		if (!collision)
		{
			//did not collide
			p_userCMDS->at(i).controller.Vibrate(0,0);
		}

	}



	return GAME_SCREEN;
}

void GameScreen::Draw()
{

}