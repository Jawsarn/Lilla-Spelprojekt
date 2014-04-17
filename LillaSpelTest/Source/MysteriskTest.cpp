#include "MysteriskTest.h"
#include "GameSetupScreen.h"

GameSetupScreen test;
MysteriskTest::MysteriskTest(void)
{
	test = GameSetupScreen();
}


MysteriskTest::~MysteriskTest(void)
{
}

void MysteriskTest::Run(std::vector<UserCMD>* players)
{
	test.Update(players);
}