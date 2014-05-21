#include "GameSetupScreen.h"


GameSetupScreen::GameSetupScreen(void)
{
}


GameSetupScreen::~GameSetupScreen(void)
{
}

GameSetupScreen::GameSetupScreen(GameInfo* p_gameInfo,GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager, std::vector<std::string> p_mapNames,std::vector<bool> p_levelMultipleLaps)
	:MenuScreen(p_gameInfo, p_graphicsHandle, p_audioManager)
{
	std::vector<UINT> t_textures;
	m_maxLaps = 5;
	UINT t_texture;
	for (int i = 0; i < p_mapNames.size(); i++)
	{
		
		std::wstring t_mapNameTexture;
		t_mapNameTexture = std::wstring(p_mapNames[i].begin(),p_mapNames[i].end());
		m_graphicHandle->LoadTexture((t_mapNameTexture + L"/nameTexture.dds").c_str(),t_texture);
		t_textures.push_back(t_texture);
	}
	UINT t_Handles;
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.7),DirectX::XMFLOAT2(0.5,0.2),t_textures,t_Handles);
	t_textures.clear();
	std::vector<UINT> t_objHandle;
	t_objHandle.push_back(t_Handles);

	for (int i = 1; i < m_maxLaps+1; i++)
	{
		char t_textureName[256];
		itoa(i,t_textureName,10);
		std::string t_string = t_textureName;
		t_string += ".dds";
		std::wstring t_wstring(t_string.begin(),t_string.end());
		m_graphicHandle->LoadTexture(t_wstring.c_str(),t_texture);
		t_textures.push_back(t_texture);
	}
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0.3,0.7),DirectX::XMFLOAT2(0.5,0.2),t_textures,t_Handles);
	t_textures.clear();
	t_objHandle.push_back(t_Handles);

	m_graphicHandle->LoadTexture(L"Laps.dds",t_texture);
	t_textures.push_back(t_texture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(-0.5,0.7),DirectX::XMFLOAT2(0.5,0.2),t_textures,t_Handles);
	t_objHandle.push_back(t_Handles);

	m_graphicHandle->CreateHudTemplate(t_objHandle,t_Handles);
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	m_graphicHandle->CreateHudFromTemplate(t_Handles,5,t_barOffsets,m_hudHandle);
	
	
	m_numberOfLaps = 1;
	currentMap = 0;
	m_levelMultipleLaps = p_levelMultipleLaps;
	m_numberOfMaps = m_graphicHandle->GetAmountOfLevels();
}


int GameSetupScreen::Update(float p_dt,std::vector<UserCMD>* userCMD )
{
	MenuScreen::Update(p_dt, userCMD);
	if (timeSinceLastChange[0]>0.5)
	{
		if (userCMD->at(0).aButtonPressed || userCMD->at(0).startButtonPressed)
		{
			SaveInfo();
			return JOIN_GAME_SCREEN;
		}
		if (userCMD->at(0).backButtonPressed)
		{
			timeSinceLastChange[0] = 0;
			return MAIN_MENU_SCREEN;
		} 
	}
	if (timeSinceLastChange[0]>0.1)
	{
		ChangeLevel(userCMD);
		ChangeNumberOfLaps(userCMD);
	}
	return GAME_SETUP_SCREEN;
}

void GameSetupScreen::Draw()
{

}

void GameSetupScreen::Initialize()
{
	m_graphicHandle->SetViewportAmount(1);
	m_graphicHandle->UseHud(0,m_hudHandle);
	m_graphicHandle->ChangeLevelSelection(currentMap);
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,0,currentMap);
	m_numberOfLaps = 1;
	MenuScreen::Initialize();
}

void GameSetupScreen::SaveInfo()
{
	m_graphicHandle->RemoveLevelDraw();
	m_gameInfo->map = currentMap;
}

void GameSetupScreen::ChangeLevel(std::vector<UserCMD>* userCMD )
{
	if (userCMD->at(0).Joystick.x>0.8)
	{
		timeSinceLastChange[0] = 0;
		if (m_numberOfMaps - 1 > currentMap)
		{
			currentMap++;	
		}
		else
		{
			currentMap=0;
		}
		m_graphicHandle->ChangeLevelSelection(currentMap);
		m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,0,currentMap);
		m_numberOfLaps=1;
		m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,1,m_numberOfLaps-1);
	}
	else if (userCMD->at(0).Joystick.x<-0.8)
	{
		timeSinceLastChange[0] = 0;
		if (0 < currentMap)
		{
			currentMap--;
		}
		else
		{
			currentMap=m_numberOfMaps -1;
		}
		m_graphicHandle->ChangeLevelSelection(currentMap);
		m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,0,currentMap);
		m_numberOfLaps=1;
		m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,1,m_numberOfLaps-1);
	}
}
void GameSetupScreen::ChangeNumberOfLaps(std::vector<UserCMD>* userCMD )
{
	if (m_levelMultipleLaps[currentMap])
	{
		if (userCMD->at(0).Joystick.y>0.8)
		{
			timeSinceLastChange[0]=0;
			m_numberOfLaps++;
			if (m_numberOfLaps==m_maxLaps + 1)
			{
				m_numberOfLaps = 1;
			}
			m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,1,m_numberOfLaps-1);
		}
		else if (userCMD->at(0).Joystick.y<-0.8)
		{
			timeSinceLastChange[0]=0;
			m_numberOfLaps--;
			if (m_numberOfLaps==0)
			{
				m_numberOfLaps = m_maxLaps;
			}
			m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,1,m_numberOfLaps-1);
		}
	}
}