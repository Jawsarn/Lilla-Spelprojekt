#include "GameScreen.h"


GameScreen::GameScreen(void)
{
}

GameScreen::GameScreen(int p_color[4], int p_whatVehicle[4],string p_tauntSound[4], std::string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle, AudioManager* p_audioManager, int p_nrOfLaps,int p_mapID)
	:Screen(p_graphicHandle, p_audioManager)
{
	m_mapTotalDistance=0;
	m_mapID = p_mapID;
	m_nrOfLaps = p_nrOfLaps+1;
	m_nrOfFinishedPlayers = 0;
	m_engineSound[0] = "Engine1.wav";
	m_engineSound[1] = "Engine2.wav";
	m_engineSound[2] = "Engine3.wav";
	m_engineSound[3] = "Engine4.wav";

	m_audioManager->CreateSound("countdown.wav");
	m_audioManager->SetSpecificSoundVolume("countdown.wav",0.1);
	m_audioManager->CreateSound("go.wav");
	m_audioManager->SetSpecificSoundVolume("go.wav",0.1);
	m_audioManager->CreateSound("game2.mp3");

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
		m_vibrationTimer[i] = 0;
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
	for (int i = 0; i < 4; i++)
	{
		m_immortalCounter[i]=0;
	}

	//calculate total length of map
	for (int i = 0; i< m_mapNodes->size();i++)
	{
		XMFLOAT3 t_distance;
		XMStoreFloat3(&t_distance, XMVector3Length(XMLoadFloat3(&m_mapNodes->at(i)->m_normal)));
		m_mapTotalDistance += t_distance.x;
	}





}

GameScreen::~GameScreen(void)
{
	////Remove sound////
	m_audioManager->RemoveSpecificSound("game2.mp3");
	////Remove Graphic Things////
	m_graphicHandle->RemoveLights();
	m_graphicHandle->RemoveLevelDraw(m_mapID);
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
	int t_finished = 0;
	for (int i = 0; i < m_players.size(); i++)
	{
		if(PauseCheck(i, p_userCMDS->at(i)) == PAUSE_SCREEN)
		{
			StopSounds();
			StopAllVibration(p_userCMDS);
			return PAUSE_SCREEN;
		}

		// next lap/win check
		if(m_players[i]->CurrentLap() >= m_nrOfLaps&&!m_players[i]->HasFinished())
		{

			m_graphicHandle->ChangeHudObjectTexture(m_hudID[i],0,m_nrOfFinishedPlayers);
			if (!m_players[i]->IsGravityShifting())
			{
				m_nrOfFinishedPlayers++;
				m_players[i]->Finish();
			}

			m_audioManager->RemoveSpecificSound(m_engineSound[i]);


			t_finished++;
		}
		else if(m_players[i]->ChangedNode()&&m_lastNodeIndex == m_players[i]->GetCurrentMapNode()->m_Index&&!m_players[i]->HasFinished())
			m_players[i]->NextLap();

		if (m_vibrationTimer[i] > 0)
		{
			UpdateVibration(p_dt,i,p_userCMDS->at(i));
		}

		//ACTUAL PROPER UPDATE BEGINS
		int t_playerUpdateResult = UpdatePlayer(i, p_dt, p_userCMDS->at(i));
		if(t_playerUpdateResult ==1) //checks if a new wall is placed
		{
			PlacePlayerWall(i);
		}

		//collision
		if ((m_players[i]->GetImmortal() && m_players[i]->GetImmortalTimer()>0)||m_players[i]->IsGravityShifting())
		{
			if (m_immortalCounter[i]>m_players[i]->GetMaxImmortalTimer())
			{
				m_immortalCounter[i] = 0;
			}
			m_immortalCounter[i]+=p_dt;

			if(cos(m_immortalCounter[i]*m_immortalCounter[i]*4)>0&&m_players[i]->GetDrawn())
			{
				m_graphicHandle->RemoveDrawPlayer(i);
				m_players[i]->SetDrawn(false);
			}
			else if(cos(m_immortalCounter[i]*m_immortalCounter[i]*4)<0&&!m_players[i]->GetDrawn())
			{
				m_graphicHandle->AddDrawPlayer(i);
				m_players[i]->SetDrawn(true);
			}


		}

		else if(!m_players[i]->GetDrawn())
		{
			m_graphicHandle->AddDrawPlayer(i);
			m_players[i]->SetDrawn(true);
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
	switch(m_state)
	{
		//pvp collision
	case PLAY:
		m_collisionManager->PlayerVsPlayer(m_players);
		for (int i = 0; i < m_players.size(); i++)
		{
			if(!m_players[i]->GetImmortal()&&!m_players[i]->HasFinished())
				CollisionCheck(i, p_dt,p_userCMDS->at(i) );
		}
		break;
	}
	//draw player
	for (int i = 0; i < m_players.size(); i++)
	{
		DrawPlayer(i);
	}
	if (m_nrOfFinishedPlayers == m_players.size())
	{
		int t_doneFinishing = 0;
		for (int i = 0; i< m_players.size();i++)
		{

			if(m_players[i]->DoneFinishing())
				t_doneFinishing++;
		}
		if(t_doneFinishing>=m_players.size())
			return GOAL_SCREEN;
	}
	return GAME_SCREEN;
}
void GameScreen::Draw()
{

}


int GameScreen::PauseCheck(int p_currentPlayer, UserCMD p_userCmd)
{
	if(p_userCmd.startButtonPressed)
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
	int t_collisionReturn = m_collisionManager->PlayerVsObj(m_players[p_currentPlayer]->GetCollisionBox(), m_wallsToCheck);
	if(t_collisionReturn==1)    
	{
		PlayerDieStaticObj(p_currentPlayer);
		p_userCMD.controller.Vibrate(60000,60000);
		m_vibrationTimer[p_currentPlayer] = 0.5;
		m_audioManager->PlaySpecificSound("crash.wav",false,AUDIO_PLAY_MULTIPLE);
	}
	else if(t_collisionReturn==0)
	{
		m_players[p_currentPlayer]->PadBoost(p_dt);
		p_userCMD.controller.Vibrate(60000,0);
		m_vibrationTimer[p_currentPlayer] = 1.5;
		//ljud?
	}

	//player vs playerwall
	vector<PlayerWall*> t_playerWallsToCheck = t_currMapNode->m_playerWalls;
	int t_collisionResult = m_collisionManager->PlayerVsPlayerWall(m_players[p_currentPlayer], t_playerWallsToCheck, p_currentPlayer);
	if(t_collisionResult == -1)	
	{
		PlayerDiePlayerWall(p_currentPlayer);
		p_userCMD.controller.Vibrate(60000,60000);
		m_vibrationTimer[p_currentPlayer] = 0.5;
		m_audioManager->PlaySpecificSound("crash.wav",false,AUDIO_PLAY_MULTIPLE);
	}
	else if (t_collisionResult>0)
	{
		PlayerCloseToWall(p_currentPlayer, t_collisionResult, p_dt);

	}
	//player vs player
	if (p_userCMD.yButtonPressed&&m_players[p_currentPlayer]->AbilityReady()&&!m_players[p_currentPlayer]->GetImmortal())
	{
		m_players[p_currentPlayer]->SetShockwaveCooldown();
		m_collisionManager->ShockWaveCollision(m_players,p_currentPlayer);
	}
}

void GameScreen::UpdatePlayerRacePosition(int p_currentPlayer)
{
	int t_racePos = 1;
	float t_currPlayerDistance = m_players.at(p_currentPlayer)->GetDistanceTraveled()+m_players[p_currentPlayer]->CurrentLap()*m_mapTotalDistance;
	for (int j = 0; j < m_players.size(); j++)
	{
		if(p_currentPlayer!=j)
		{
			float t_distanceToCheck = m_players[j]->GetDistanceTraveled()+m_players[j]->CurrentLap()*m_mapTotalDistance;
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

	if (m_players[p_player]->CurrentLap() < m_nrOfLaps && !m_players[p_player]->HasFinished())
	{
		int t_laps = m_players[p_player]->CurrentLap()-1 >= m_nrOfLaps - 1 ? m_nrOfLaps - 2 : m_players[p_player]->CurrentLap()-1;
		m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_player],4,t_laps);
		m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_player],0,m_players[p_player]->GetRacePosition()-1);
	}
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],1,DirectX::XMFLOAT2(m_players[p_player]->GetHudBoosterInfo(),0));
	float t_wallMeter = m_players[p_player]->GetHudWallInfo() <= 0 ? 0 : m_players[p_player]->GetHudWallInfo();
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],2,DirectX::XMFLOAT2(t_wallMeter,0));
	m_graphicHandle->UpdateHudBarOffset(m_hudID[p_player],7,DirectX::XMFLOAT2(m_players[p_player]->GetRemainingAbilityCooldown(),0));
	float t_whatCDTexture = 1 - m_players[p_player]->GetRemainingAbilityCooldown();
	m_graphicHandle->ChangeHudObjectTexture(m_hudID[p_player],7,(int)t_whatCDTexture);
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
	if (p_userCMDS->at(p_player).leftRetardButtonPressed)
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
	m_audioManager->PlaySpecificSound("game2.mp3",true,AUDIO_ONLY_PLAY_ONE);
}

void GameScreen::StopSounds()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_audioManager->StopSpecificSound(m_engineSound[i]);
	}
	m_audioManager->PauseSpecificSound("game2.mp3");
}

void GameScreen::StopAllVibration(std::vector<UserCMD>* p_userCMDS)
{
	for (int i = 0; i < p_userCMDS->size(); i++)
	{
		p_userCMDS->at(i).controller.Vibrate(0,0);
	}
}

void GameScreen::UpdateVibration(float p_dt,int p_player, UserCMD& p_userCMD)
{
	m_vibrationTimer[p_player] -= p_dt;
	if (m_vibrationTimer[p_player]<=0)
	{
		p_userCMD.controller.Vibrate(0,0);
	}
}

void GameScreen::PlayShockWaveSound()
{
	m_audioManager->PlaySpecificSound("waca.wav",false,AUDIO_PLAY_MULTIPLE);
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
	UINT t_lapsHandle;
	UINT t_coolDownHandle;
	UINT t_coolDownIconHandle;
	UINT t_boostIconHandle;
	UINT t_wallIconHandle;
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
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"BoostSlide.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.8),DirectX::XMFLOAT2(0.8,0.02),t_textureIDs,t_boostBarHandle);
	t_hudParts.push_back(t_boostBarHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(1,0));
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"WallSlide.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.9),DirectX::XMFLOAT2(0.8,0.02),t_textureIDs,t_wallBarHandle);
	t_hudParts.push_back(t_wallBarHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(1,0));
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"CoolDownSlide1.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->LoadTexture(L"CoolDownSlide2.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.7),DirectX::XMFLOAT2(0.8,0.02),t_textureIDs,t_coolDownHandle);

	//For icons next to the correct bars
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"BoostIcon.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(-0.9,-0.8),DirectX::XMFLOAT2(0.035,0.035),t_textureIDs,t_boostIconHandle);
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"WallIcon.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(-0.9,-0.9),DirectX::XMFLOAT2(0.035,0.035),t_textureIDs,t_wallIconHandle);
	t_textureIDs.clear();
	m_graphicHandle->LoadTexture(L"CooldownIcon.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(-0.9,-0.7),DirectX::XMFLOAT2(0.035,0.035),t_textureIDs,t_coolDownIconHandle);

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

	t_textureIDs.clear();

	//comment back for lap numbers and comment back in DrawHud, need to fix positioning 

	for (int i = 1; i < m_nrOfLaps; i++)
	{
		char t_textureName[256];
		itoa(i,t_textureName,10);
		std::string t_string = t_textureName;
		t_string += ".dds";
		std::wstring t_wstring(t_string.begin(),t_string.end());
		m_graphicHandle->LoadTexture(t_wstring.c_str(),t_texture);
		t_textureIDs.push_back(t_texture);
	}
	//for number of laps you have driven
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0.81,0.8),DirectX::XMFLOAT2(0.1,0.1),t_textureIDs,t_lapsHandle);
	t_hudParts.push_back(t_lapsHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	//for total nubmer of laps
	t_textureIDs.erase(t_textureIDs.begin(),t_textureIDs.end()-1);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0.9,0.8),DirectX::XMFLOAT2(0.1,0.1),t_textureIDs,t_lapsHandle);
	t_hudParts.push_back(t_lapsHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));

	t_textureIDs.clear();
	//for the slash in between
	m_graphicHandle->LoadTexture(L"slash.dds",t_texture);
	t_textureIDs.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0.85,0.8),DirectX::XMFLOAT2(0.1,0.1),t_textureIDs,t_lapsHandle);
	t_hudParts.push_back(t_lapsHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));

	//for the ability cooldown. nr 7
	t_hudParts.push_back(t_coolDownHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(1,0));

	//for the icons. 8,9,10
	t_hudParts.push_back(t_boostIconHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	t_hudParts.push_back(t_wallIconHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	t_hudParts.push_back(t_coolDownIconHandle);
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