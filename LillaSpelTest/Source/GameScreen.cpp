#include "GameScreen.h"


GameScreen::GameScreen(void)
{
}

GameScreen::GameScreen(int p_color[4], int p_whatVehicle[4],string p_tauntSound[4], std::string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle, AudioManager* p_audioManager )
	:Screen(p_graphicHandle, p_audioManager)
{
	m_engineSound[0] = "Engine1.wav";
	m_engineSound[1] = "Engine2.wav";
	m_engineSound[2] = "Engine3.wav";
	m_engineSound[3] = "Engine4.wav";
	m_state = COUNTDOWN;
	m_mapLoader = new MapLoader();
	m_mapNodes = m_mapLoader->LoadMap(p_mapName);
	m_timeSpentDuringPreUpdate = 0;
	//vector<XMFLOAT3> t_centerSplinePositions = m_mapLoader->LoadLogicalObj("centerspline").at(0);
	vector<XMFLOAT3> t_centerSplinePositions = m_mapLoader->LoadLogicalObj(p_mapName+"/CenterSpline.obj").at(0);
	m_graphicHandle->CreateMapLights(t_centerSplinePositions);

	m_lastNodeIndex = m_mapNodes->at(m_mapNodes->size()-1)->m_Index;
	vector<XMFLOAT4X4> t_shipWorldMatrices;
	vector<UINT> t_colors;
	vector<UINT> t_whichVehicles;

	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		m_players.push_back(new Player(m_mapNodes->at(0),0.5*3.1415*i, i));
		XMFLOAT4X4 t_TempWorld;
		XMStoreFloat4x4( &t_TempWorld, m_players[i]->GetWorldMatrix());
		t_shipWorldMatrices.push_back(t_TempWorld);
		t_colors.push_back(p_color[i]);
		t_whichVehicles.push_back(p_whatVehicle[i]);
		m_tauntSound[i] = p_tauntSound[i];
		m_audioManager->CreateSound(m_tauntSound[i]);
		m_audioManager->CreateSound(m_engineSound[i]);
	}

	m_graphicHandle->SetAmountOfPlayers(p_numberOfPlayers);
	m_graphicHandle->SetColourAndVehicle(t_colors, t_whichVehicles);
	m_graphicHandle->CreateShipForGame(t_shipWorldMatrices);
	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		DrawPlayer(i);
	}
	m_collisionManager = new CollisionManager();
	m_preUpdateCountdown = 0;
	PlaySounds();
	CreatePlayerHUDs(p_numberOfPlayers,p_color, p_mapName);
}

GameScreen::~GameScreen(void)
{
	////Remove Graphic Things////
	m_graphicHandle->RemoveLevelDraw();
	m_graphicHandle->RemovePlayers();
	std::vector<PlayerWall*>* t_playerWalls;
	int t_wallListSize;
	for (int i = 0; i < m_players.size(); i++)
	{
		t_playerWalls = m_players[i]->GetPlacedWalls();
		t_wallListSize = t_playerWalls->size();
		for (int j = 0; j < t_wallListSize; j++)
		{
			unsigned int t_wallToRemove = t_playerWalls->at(j)->GetWallIndex();
			m_graphicHandle->RemoveObject(t_wallToRemove);
		}
	}

	////Remove logical stuff for reduced memory leakage////
	for (int i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
		m_players[i] = nullptr;
	}
	delete m_mapLoader;
}
void GameScreen::Initialize()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_graphicHandle->UseHud(i,m_hudID[i]);
	}
	PlaySounds();
}

void GameScreen::PreUpdate(float p_dt, std::vector<UserCMD>* p_userCMDS, int p_Player)
{
	m_timeSpentDuringPreUpdate +=(p_dt/m_players.size());
	if (m_preUpdateCountdown != (int)m_timeSpentDuringPreUpdate && !(m_timeSpentDuringPreUpdate >=4))
	{
		m_audioManager->PlaySpecificSound("countdown.wav",false,AUDIO_ONLY_PLAY_ONE);
	}
	if(m_timeSpentDuringPreUpdate>=1 && m_timeSpentDuringPreUpdate <2)
	{
		m_preUpdateCountdown = 1;
		m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_Player],3,1); 


	}
	else if(m_timeSpentDuringPreUpdate >=2 && m_timeSpentDuringPreUpdate <3)
	{
		m_preUpdateCountdown = 2;
		m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_Player],3,2);


	}
	else if(m_timeSpentDuringPreUpdate >=3 && m_timeSpentDuringPreUpdate <4)
	{
		m_preUpdateCountdown = 3;
		m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_Player],3,3);


	}
	else if(m_timeSpentDuringPreUpdate >=4)
	{		
		for (int i = 0; i < m_players.size(); i++)
		{
			m_graphicHandle->ChangeHudObjectTexture(m_hudID[i],3,4);
			m_players[i]->Start();
		}


		m_state = PLAY;
		m_audioManager->PlaySpecificSound("go.wav",false,AUDIO_ONLY_PLAY_ONE);
	}





}

int GameScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDS) 
{
	for (int i = 0; i < m_players.size(); i++)
	{
		if(PauseCheck(i, p_userCMDS->at(i)) == PAUSE_SCREEN)
		{
			StopSounds();
			return PAUSE_SCREEN;
		}
		//first, semi-underdeveloped win condition check
		if (m_lastNodeIndex != m_players[i]->GetCurrentMapNode()->m_Index)
		{
			//i don't even..



			//ACTUAL PROPER UPDATE BEGINS
			int t_playerUpdateResult = UpdatePlayer(i, p_dt, p_userCMDS->at(i));
			if(t_playerUpdateResult ==1) //checks if a new wall is placed
			{
				PlacePlayerWall(i);
			}

			switch(m_state)
			{
			case COUNTDOWN:
				PreUpdate(p_dt, p_userCMDS,i);
				break;
			case PLAY:
				UpdatePlayerRacePosition(i);
				DrawPlayerHUD(i);
				UpdateSounds(i,p_userCMDS);
				break;
			}

		}
		else
		{
			m_audioManager->RemoveSpecificSound(m_engineSound[i]);
			m_players[i]->SetFinalDirection();
		}
	}
	switch(m_state)
	{
	case PLAY:
		m_collisionManager->PlayerVsPlayer(m_players);
		for (int i = 0; i < m_players.size(); i++)
		{
			if(!m_players[i]->GetImmortal())
				CollisionCheck(i, p_dt,p_userCMDS->at(i) );
		}
		break;
	}
	for (int i = 0; i < m_players.size(); i++)
	{
		DrawPlayer(i);
	}
	return GAME_SCREEN;
}
void GameScreen::Draw()
{

}


int GameScreen::PauseCheck(int p_currentPlayer, UserCMD p_userCmd)
{
	if(p_userCmd.backButtonPressed)
	{
		m_pauseDudeIndex = p_currentPlayer;
		return PAUSE_SCREEN;
	}
}

int GameScreen::UpdatePlayer(int p_currentPlayer, float p_dt, UserCMD p_userCmd)
{
	return m_players[p_currentPlayer]->ProperUpdatePosition(p_dt, p_userCmd);
}

void GameScreen::PlacePlayerWall(int p_currentPlayer)
{
	PlayerWall* t_newWall = m_players[p_currentPlayer]->GetLastPlacedWall();
	t_newWall->m_wallIndex = m_graphicHandle->CreateWall(0, t_newWall->GetWorldMatrix(), p_currentPlayer);
}

void GameScreen::CollisionCheck(int p_currentPlayer, float p_dt, UserCMD& p_userCMD)
{
	MapNode* t_currMapNode = m_players[p_currentPlayer]->GetCurrentMapNode(); 
	//player vs static obj
	vector<StaticObj*>* m_wallsToCheck = &t_currMapNode->m_staticObjs;		
	if(m_wallsToCheck->size()>0)
	{
		int derp = 0;
		derp++;

	}
	if(m_collisionManager->PlayerVsObj(m_players[p_currentPlayer]->GetCollisionBox(), m_wallsToCheck)!=-1)    
	{
		PlayerDieStaticObj(p_currentPlayer);
	}

	//player vs playerwall
	vector<PlayerWall*> t_playerWallsToCheck = t_currMapNode->m_playerWalls;
	int t_collisionResult = m_collisionManager->PlayerVsPlayerWall(m_players[p_currentPlayer]->GetCollisionBox(), t_playerWallsToCheck, p_currentPlayer);
	if(t_collisionResult == -1)	
	{
		PlayerDiePlayerWall(p_currentPlayer);
	}
	else if (t_collisionResult>0)
	{
		PlayerCloseToWall(p_currentPlayer, t_collisionResult, p_dt);
	}
	//player vs player
	if (p_userCMD.yButtonPressed)
	{
		m_collisionManager->ShockWaveCollision(m_players,p_currentPlayer);
	}
}

void GameScreen::UpdatePlayerRacePosition(int p_currentPlayer)
{
	int t_racePos = 1;
	float t_currPlayerDistance = m_players.at(p_currentPlayer)->GetDistanceTraveled();
	for (int j = 0; j < m_players.size(); j++)
	{
		if(p_currentPlayer!=j)
		{
			float t_distanceToCheck = m_players[j]->GetDistanceTraveled();
			if(t_currPlayerDistance < t_distanceToCheck)
			{
				t_racePos++;
			}
		}
		m_players[p_currentPlayer]->SetPlayerRacePosition(t_racePos);
	}
}

void GameScreen::DrawPlayer(int p_currentPlayer)
{
	m_graphicHandle->JohnSetCamera(m_players[p_currentPlayer]->GetWorldMatrix(), p_currentPlayer);
	m_graphicHandle->UpdatePlayer(p_currentPlayer, m_players[p_currentPlayer]->GetWorldMatrix(), m_players[p_currentPlayer]->GetCamMatrix());
}

void GameScreen::DrawPlayerHUD(int p_player)
{
	m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_player],0,m_players[p_player]->GetRacePosition()-1);
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],1,DirectX::XMFLOAT2(m_players[p_player]->GetHudBoosterInfo(),0));
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],2,DirectX::XMFLOAT2(m_players[p_player]->GetHudWallInfo(),0));
}


void GameScreen::PlayerDieStaticObj(int p_currentPlayer)
{
	m_players.at(p_currentPlayer)->Die();
}

void GameScreen::PlayerDiePlayerWall(int p_currentPlayer)
{
	m_players.at(p_currentPlayer)->Die();
}

void GameScreen::PlayerCloseToWall(int p_currentPlayer, int p_wallsCloseTo, float p_dt)
{
	m_players.at(p_currentPlayer)->IncreaseBoost(p_wallsCloseTo, p_dt);
}

void GameScreen::UpdateSounds(int p_player,std::vector<UserCMD>* p_userCMDS)
{
	if (p_userCMDS->at(p_player).leftBumberPressed)
	{
		m_audioManager->PlaySpecificSound(m_tauntSound[p_player],false,AUDIO_ONLY_PLAY_ONE);
	}
	float t_pitchCoefficient = (m_players[p_player]->GetSpeed() < 25 ? m_players[p_player]->GetSpeed() : 25);
	m_audioManager->PitchSpecificSound(m_engineSound[p_player], std::sqrt( t_pitchCoefficient) * 15000);
}

void GameScreen::PlaySounds()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_audioManager->PlaySpecificSound(m_engineSound[i],true,AUDIO_ONLY_PLAY_ONE);
		m_audioManager->PitchSpecificSound(m_engineSound[i],0);
	}
}

void GameScreen::StopSounds()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_audioManager->StopSpecificSound(m_engineSound[i]);
	}
}

void GameScreen::CreatePlayerHUDs(int p_numberOfPlayers, int p_color[4], std::string p_mapName)
{
	m_hudID.resize(p_numberOfPlayers,0);
	UINT t_placementHandle;
	UINT t_texture;
	UINT t_templateHandle;
	UINT t_boostBarHandle;
	UINT t_wallBarHandle;
	UINT t_MiedelHandle;
	std::vector<UINT> t_hudParts;
	std::vector<UINT> t_textureIDs;
	std::vector<UINT> t_textureCountDownID;
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

	std::wstring t_countdownTexture;
	t_countdownTexture = wstring(p_mapName.begin(),p_mapName.end());
	m_graphicHandle->LoadTexture((t_countdownTexture + L"/3.dds").c_str(),t_texture);
	t_textureCountDownID.push_back(t_texture);
	m_graphicHandle->LoadTexture((t_countdownTexture + L"/2.dds").c_str(),t_texture);
	t_textureCountDownID.push_back(t_texture);
	m_graphicHandle->LoadTexture((t_countdownTexture + L"/1.dds").c_str(),t_texture);
	t_textureCountDownID.push_back(t_texture);
	m_graphicHandle->LoadTexture((t_countdownTexture + L"/gu.dds").c_str(),t_texture);
	t_textureCountDownID.push_back(t_texture);
	m_graphicHandle->LoadTexture(L"Nothing.dds",t_texture);
	t_textureCountDownID.push_back(t_texture);


	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0.0,0.0),DirectX::XMFLOAT2(0.2,0.2),t_textureCountDownID,t_MiedelHandle);
	t_hudParts.push_back(t_MiedelHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));

	m_graphicHandle->CreateHudTemplate(t_hudParts,t_templateHandle);

	for (int i = 0; i < p_numberOfPlayers; i++)
	{
		m_graphicHandle->CreateHudFromTemplate(t_templateHandle,p_color[i],t_barOffsets,m_hudID[i]);
		m_graphicHandle->UseHud(i,m_hudID[i]);
	}

}
float testaren = 1;

int GameScreen::GetPauseDudeIndex()
{
	return m_pauseDudeIndex;
}