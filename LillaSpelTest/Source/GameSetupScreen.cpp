#include "GameSetupScreen.h"


GameSetupScreen::GameSetupScreen(void)
{
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
	return 1;
}
void GameSetupScreen::Draw()
{}
