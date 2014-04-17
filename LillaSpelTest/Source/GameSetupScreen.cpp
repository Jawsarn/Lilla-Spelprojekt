#include "GameSetupScreen.h"


GameSetupScreen::GameSetupScreen(void)
{
	AddButton("Start", DirectX::XMFLOAT2(0,-0.7),0.1,0.1);
	AddButton("Next map", DirectX::XMFLOAT2(0.5,-0.7),0.1,0.1);
	AddButton("Prev map", DirectX::XMFLOAT2(-0.5,-0.7),0.1,0.1);

	FixButtonPointers();
	currentButton = buttonList[0];
	currentMap = 0;
}


GameSetupScreen::~GameSetupScreen(void)
{
}

GameSetupScreen::GameSetupScreen(GameInfo* p_gameInfo)
	:MenuScreen(p_gameInfo)
{
	
}


int GameSetupScreen::Update(std::vector<UserCMD>* userCMD)
{
	std::string t_menuChoice = NavigateMenu(userCMD->at(0));
	if (t_menuChoice == "Start")
	{
		return GAME_SCREEN;
	}
	else if (t_menuChoice == "Next map")
	{
		if (mapList.size() - 1 > currentMap)
		{
			currentMap++;
		}
	}
	else if (t_menuChoice == "Prev map")
	{
		if (0 < currentMap)
		{
			currentMap--;
		}
	}
	if (userCMD->at(0).backButtonPressed)
	{
		return MAIN_MENU_SCREEN;
	}
	return GAME_SETUP_SCREEN;
}

void GameSetupScreen::Draw()
{

}
