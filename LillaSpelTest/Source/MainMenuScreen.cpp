#include "MainMenuScreen.h"


MainMenuScreen::MainMenuScreen(void)
	:MenuScreen()
{
	AddButton("Start Game", DirectX::XMFLOAT2(0,0.5),0.5,0.2);
	AddButton("Options",DirectX::XMFLOAT2(0,0),0.5,0.2);
	AddButton("Quit",DirectX::XMFLOAT2(0,-0.5),0.5,0.2);

	FixButtonPointers();
	currentButton = buttonList[0];
}


MainMenuScreen::~MainMenuScreen(void)
{
}

int MainMenuScreen::Update(std::vector<UserCMD>* p_userCMD, float p_dt)
{
	MenuScreen::Update(p_userCMD,p_dt);
	std::string t_menuChoice = NavigateMenu(p_userCMD->at(0));
	if (t_menuChoice == "Start Game")
	{
		return GAME_SETUP_SCREEN;
	}
	else if (t_menuChoice == "Options")
	{
		return OPTIONS_SCREEN;
	}
	else if (t_menuChoice == "Quit")
	{
		return SHUT_DOWN;
	}

	return MAIN_MENU_SCREEN;
}

void MainMenuScreen::Draw()
{}
