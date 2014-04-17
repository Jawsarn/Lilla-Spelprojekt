#include "JoinGameScreen.h"


JoinGameScreen::JoinGameScreen(void)
{
}


JoinGameScreen::~JoinGameScreen(void)
{
}

int JoinGameScreen::Update(std::vector<UserCMD>* userCMD)
{
	for (int i = 0; i < 4; i++)
	{
		if (userCMD->at(i).aButtonPressed)
		{
			players[i] = CHOOSE_MODDEL;
		}
		else if (userCMD->at(i).bButtonPressed)
		{
			players[i] = DISCONNECTED;
		}
		if (userCMD->at(i).xButtonPressed && players[i] == CHOOSE_MODDEL || players[i] == CHOOSE_COLOR)
		{
			players[i] = READY;
		}
		if (userCMD->at(i).yButtonPressed && players[i] == READY)
		{
			players[i] = CHOOSE_MODDEL;
		}
		if (userCMD->at(i).backButtonPressed)
		{
			return GAME_SETUP_SCREEN;
		}

		if (players[i]=CHOOSE_MODDEL)
		{
			if (userCMD->at(i).Joystick.x < -0.8)
			{
				if (model[i] > 0)
				{
					model[i]--;
				}
				
			}
			else if (userCMD->at(i).Joystick.x > 0.8)
			{
				if (model[i] < NUMBER_OF_MODELLS - 1)
				{
					model[i]++;
				}	
			}
		}
	}

	return 1;
}
void JoinGameScreen::Draw()
{

}