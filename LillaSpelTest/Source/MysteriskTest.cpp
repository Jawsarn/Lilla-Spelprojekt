#include "MysteriskTest.h"
#include "GameSetupScreen.h"
#include "JoinGameScreen.h"
#include "Struct_GameInfo.h"
#include "MainMenuScreen.h"
#include <string>


ApplicationState state;

MenuScreen* gameSetup;
MenuScreen* joinGame;
MenuScreen* mainMenu;


int soundNumber1 = 0;
int soundNumber2 = 0;

GameInfo gameInfo;
GameSetupScreen setuptest;
JoinGameScreen jointest;
MysteriskTest::MysteriskTest(void)
{
	//setuptest = GameSetupScreen();
	//jointest = JoinGameScreen();
	//gameSetup = new GameSetupScreen(&gameInfo);
	//joinGame = new JoinGameScreen(&gameInfo);
	//mainMenu = new MainMenuScreen();
	//state = JOIN_GAME_SCREEN;
}

MysteriskTest::MysteriskTest(GraphicHandle* grphandle)
{
	gameSetup = new GameSetupScreen(&gameInfo, grphandle);
	joinGame = new JoinGameScreen(&gameInfo, grphandle);
	mainMenu = new MainMenuScreen(grphandle);
	state = JOIN_GAME_SCREEN;
}


MysteriskTest::~MysteriskTest(void)
{
}

void MysteriskTest::Run(std::vector<UserCMD>* players, float dt)
{
	switch (state)
	{
	case GAME_SETUP_SCREEN:
		state = (ApplicationState)gameSetup->Update(players,dt);
		break;
	case PAUSE_SCREEN:
		break;
	case GAME_SCREEN:
		state = GAME_SCREEN;
		break;
	case JOIN_GAME_SCREEN:
		state = (ApplicationState)joinGame->Update(players,dt);
		break;
	case OPTIONS_SCREEN:
		break;
	case MAIN_MENU_SCREEN:
		state = (ApplicationState)mainMenu->Update(players,dt);
		break;
	case SHUT_DOWN:
		exit(1337);
		break;
	default:
		break;
	}
}