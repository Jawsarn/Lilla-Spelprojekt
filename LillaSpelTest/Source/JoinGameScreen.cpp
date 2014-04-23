#include "JoinGameScreen.h"


JoinGameScreen::JoinGameScreen(void)
{
	
	for (int i = 0; i < 4; i++)
	{
		modell[i] = 0;
		color[i] = 0;
		playerStatus[i] = DISCONNECTED;
	}
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
			if (playerStatus[i] == CHOOSE_MODELL)
			{
				playerStatus[i]=CHOOSE_COLOR;
			}
			else if (playerStatus[i] == CHOOSE_COLOR)
			{
				playerStatus[i]=READY;
			}
			else if (playerStatus[i] == DISCONNECTED)
			{
				playerStatus[i]=CHOOSE_MODELL;
			}
		}
		else if (userCMD->at(i).backButtonPressed)
		{
			playerStatus[i] = DISCONNECTED;
		}
		if (userCMD->at(i).xButtonPressed && (playerStatus[i] == CHOOSE_MODELL || playerStatus[i] == CHOOSE_COLOR))
		{
			playerStatus[i] = READY;
		}
		if (userCMD->at(i).bButtonPressed)
		{
			if (playerStatus[i] == READY)
			{
				playerStatus[i]=CHOOSE_COLOR;
			}
			else if (playerStatus[i] == CHOOSE_COLOR)
			{
				playerStatus[i]=CHOOSE_MODELL;
			}
			else if (playerStatus[i] == CHOOSE_MODELL)
			{
				return GAME_SETUP_SCREEN;
			}	
		}

		if (playerStatus[i]==CHOOSE_MODELL)
		{
			if (userCMD->at(i).Joystick.x < -0.8)
			{
				if (modell[i] > 0)
				{
					modell[i]--;
				}
				
			}
			else if (userCMD->at(i).Joystick.x > 0.8)
			{
				if (modell[i] < NUMBER_OF_MODELLS - 1)
				{
					modell[i]++;
				}	
			}
		}
		if (playerStatus[i]==CHOOSE_COLOR)
		{
			if (userCMD->at(i).Joystick.x < -0.8)
			{
				if (color[i] > 0)
				{
					color[i]--;
				}
				
			}
			else if (userCMD->at(i).Joystick.x > 0.8)
			{
				if (color[i] < NUMBER_OF_COLORS - 1)
				{
					color[i]++;
				}	
			}
		}

		if (userCMD->at(i).startButtonPressed)
		{
			int t_numberOfPlayersReady = 0;
			int t_numberOfPlayersDisconnected = 0;
			for (int i = 0; i < 4; i++)
			{
				if (playerStatus[i]== READY)
				{
					t_numberOfPlayersReady++;
				}
				else if (playerStatus[i]== DISCONNECTED)
				{
					t_numberOfPlayersReady++;
					t_numberOfPlayersDisconnected++;
				}
				else
				{
					break;
				}
			}
			if (t_numberOfPlayersReady == 4 && t_numberOfPlayersDisconnected != 0)
			{
				return GAME_SCREEN;
			}
		}
	}

	return 1;
}
void JoinGameScreen::Draw()
{

}