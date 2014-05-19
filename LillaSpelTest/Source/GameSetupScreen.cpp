#include "GameSetupScreen.h"


GameSetupScreen::GameSetupScreen(void)
{
}


GameSetupScreen::~GameSetupScreen(void)
{
}

GameSetupScreen::GameSetupScreen(GameInfo* p_gameInfo,GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager, std::vector<std::string> p_mapNames)
	:MenuScreen(p_gameInfo, p_graphicsHandle, p_audioManager)
{
	std::vector<UINT> t_mapNameTextures;
	for (int i = 0; i < p_mapNames.size(); i++)
	{
		UINT t_texture;
		std::wstring t_mapNameTexture;
		t_mapNameTexture = std::wstring(p_mapNames[i].begin(),p_mapNames[i].end());
		m_graphicHandle->LoadTexture((t_mapNameTexture + L"/nameTexture.dds").c_str(),t_texture);
		t_mapNameTextures.push_back(t_texture);
	}
	UINT t_Handles;
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,-0.7),DirectX::XMFLOAT2(0.5,0.2),t_mapNameTextures,t_Handles);
	std::vector<UINT> t_objHandle;
	t_objHandle.push_back(t_Handles);
	m_graphicHandle->CreateHudTemplate(t_objHandle,t_Handles);
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	m_graphicHandle->CreateHudFromTemplate(t_Handles,5,t_barOffsets,m_hudHandle);

	currentMap = 0;

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
		else if (userCMD->at(0).Joystick.x>0.8)
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
		}
		if (userCMD->at(0).backButtonPressed)
		{
			timeSinceLastChange[0] = 0;
			return MAIN_MENU_SCREEN;
		} 
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
	MenuScreen::Initialize();
}

void GameSetupScreen::SaveInfo()
{
	m_graphicHandle->RemoveLevelDraw();
	m_gameInfo->map = currentMap;
}