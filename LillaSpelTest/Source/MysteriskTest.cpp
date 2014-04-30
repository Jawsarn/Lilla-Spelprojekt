#include "MysteriskTest.h"
#include "GameSetupScreen.h"
#include "JoinGameScreen.h"
#include "Struct_GameInfo.h"
#include "MainMenuScreen.h"
#include "OptionsScreen.h"
#include <string>


ApplicationState state;

MenuScreen* gameSetup;
MenuScreen* joinGame;
MenuScreen* mainMenu;
MenuScreen* options;

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
	options = new OptionsScreen(&gameInfo,grphandle);
	state = MAIN_MENU_SCREEN;
}


MysteriskTest::~MysteriskTest(void)
{
}

//void RunInitialization()
//{
//	switch (state)
//	{
//	case GAME_SETUP_SCREEN:
//		gameSetup->Initialize();
//		break;
//	case PAUSE_SCREEN:
//		break;
//	case GAME_SCREEN:
//		break;
//	case JOIN_GAME_SCREEN:
//		joinGame->Initialize();
//		break;
//	case OPTIONS_SCREEN:
//		options->Initialize();
//		break;
//	case MAIN_MENU_SCREEN:
//		mainMenu->Initialize();
//		break;
//	case SHUT_DOWN:
//		break;
//	default:
//		break;
//	}
//}

void MysteriskTest::Run(std::vector<UserCMD>* players, float dt)
{
	/*switch (state)
	{
	case GAME_SETUP_SCREEN:
		state = (ApplicationState)gameSetup->Update(dt,players);
		if (state != GAME_SETUP_SCREEN)
		{
			RunInitialization();
		}
		break;
	case PAUSE_SCREEN:
		break;
	case GAME_SCREEN:
		state = GAME_SCREEN;
		break;
	case JOIN_GAME_SCREEN:
		state = (ApplicationState)joinGame->Update(dt,players);
		if (state != JOIN_GAME_SCREEN)
		{
			RunInitialization();
		}
		break;
	case OPTIONS_SCREEN:
		state = (ApplicationState)options->Update(dt,players);
		if (state != OPTIONS_SCREEN)
		{
			RunInitialization();
		}
		break;
	case MAIN_MENU_SCREEN:
		state = (ApplicationState)mainMenu->Update(dt,players);
		if (state != MAIN_MENU_SCREEN)
		{
			RunInitialization();
		}
		break;
	case SHUT_DOWN:
		exit(1337);
		break;
	default:
		break;
	}*/
}