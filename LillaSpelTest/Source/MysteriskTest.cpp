#include "MysteriskTest.h"
#include "GameSetupScreen.h"
#include "JoinGameScreen.h"

GameSetupScreen setuptest;
JoinGameScreen jointest;
MysteriskTest::MysteriskTest(void)
{
	setuptest = GameSetupScreen();
	jointest = JoinGameScreen();
}


MysteriskTest::~MysteriskTest(void)
{
}

void MysteriskTest::Run(std::vector<UserCMD>* players)
{
	setuptest.Update(players);
	jointest.Update(players);
}