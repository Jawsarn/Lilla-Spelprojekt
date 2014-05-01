#include "GameSetupScreen.h"


GameSetupScreen::GameSetupScreen(void)
{
}


GameSetupScreen::~GameSetupScreen(void)
{
}

GameSetupScreen::GameSetupScreen(GameInfo* p_gameInfo,GraphicHandle* p_graphicsHandle)
	:MenuScreen(p_gameInfo, p_graphicsHandle)
{
	AddButton("Start", DirectX::XMFLOAT2(0,-0.7),0.1,0.1, L"Start1.dds",nullptr);

	FixButtonPointers();
	MakeHud(m_hudHandle);
	currentButton = buttonList[0];
	currentMap = 0;

	m_numberOfMaps = m_graphicHandle->GetAmountOfLevels();
}


int GameSetupScreen::Update(float p_dt,std::vector<UserCMD>* userCMD )
{
	MenuScreen::Update(p_dt, userCMD);
	if (timeSinceLastChange[0]>0.5)
	{
		std::string t_menuChoice = NavigateMenu(userCMD->at(0),m_hudHandle);
		if (t_menuChoice == "Start" || userCMD->at(0).startButtonPressed)
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
}

void GameSetupScreen::SaveInfo()
{
	m_gameInfo->map = currentMap;
}