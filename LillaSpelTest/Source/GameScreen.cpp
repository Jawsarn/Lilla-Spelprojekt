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

	CreatePlayerHUDs(p_numberOfPlayers,p_color);
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
		/////////////////UPDATE PLAYER POSITION AND DROP WALLS/////////////////
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


		////////COLLISION CHECKS///////////
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
				t_playerWallsToCheck.pop_back();														
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
			float t_currentBoost = m_players[i]->GetPlayerBoost();
			m_players[i]->SetPlayerBoost(t_currentBoost+p_dt*t_collisionResult*t_boostPerWallPerUpdate);
			p_userCMDS->at(i).controller.Vibrate(10000,10000);
		}
		if (!collision)
		{
			//did not collide
			p_userCMDS->at(i).controller.Vibrate(0,0);
		}

		UpdatePlayerHUD(i);
	}

	//Give all players their respective racePosition by checking ever player vs every other player
	
	for (int i = 0; i < m_players.size(); i++)
	{
		float t_currPlayerDistance = m_players[i]->GetDistanceTraveled();
		int t_racePos = 1;
		for (int j = 0; j < m_players.size(); j++)
		{
			if(i!=j)
			{
				float t_distanceToCheck = m_players[j]->GetDistanceTraveled();
				if(t_currPlayerDistance < t_distanceToCheck)
				{
					t_racePos++;
				}

			}
		}
		m_players[i]->SetPlayerRacePosition(t_racePos);
	}


	return GAME_SCREEN;
}

void GameScreen::Draw()
{

}

void GameScreen::CreatePlayerHUDs(int p_numberOfPlayers, int p_color[4])
{
	m_hudID.resize(p_numberOfPlayers,0);
	UINT t_placementHandle;
	UINT t_texture;
	UINT t_templateHandle;
	UINT t_boostBarHandle;
	UINT t_wallBarHandle;
	std::vector<UINT> t_hudParts;
	std::vector<UINT> t_textureIDs;
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	m_graphicHandle->LoadTexture(L"first.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->LoadTexture(L"second.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->LoadTexture(L"third.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->LoadTexture(L"fourth.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(-0.8,0.8),DirectX::XMFLOAT2(0.1,0.1),t_textureIDs,t_placementHandle);
	t_hudParts.push_back(t_placementHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.8),DirectX::XMFLOAT2(1,0.02),t_textureIDs,t_boostBarHandle);
	t_hudParts.push_back(t_boostBarHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.9),DirectX::XMFLOAT2(1,0.02),t_textureIDs,t_wallBarHandle);
	t_hudParts.push_back(t_wallBarHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	m_graphicHandle->CreateHudTemplate(t_hudParts,t_templateHandle);
	
	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		m_graphicHandle->CreateHudFromTemplate(t_templateHandle,p_color[i],t_barOffsets,m_hudID[i]);
		m_graphicHandle->UseHud(i,m_hudID[i]);
	}
	
}
float testaren = 1;
void GameScreen::UpdatePlayerHUD(int p_player)
{
	m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_player],0,m_players[p_player]->GetRacePosition()-1);
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],1,DirectX::XMFLOAT2(m_players[p_player]->GetHudBoosterInfo(),0));
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],2,DirectX::XMFLOAT2(m_players[p_player]->GetHudWallInfo(),0));
}