#include "JoinGameScreen.h"


JoinGameScreen::JoinGameScreen(void)
{
	
	for (int i = 0; i < 4; i++)
	{
		m_modell[i] = 0;
		m_color[i] = 0;
		m_playerStatus[i] = DISCONNECTED;
	}
}

JoinGameScreen::JoinGameScreen(GameInfo* p_gameInfo)
	:MenuScreen(p_gameInfo)
{
	for (int i = 0; i < 4; i++)
	{
		m_modell[i] = 0;
		m_color[i] = 0;
		m_playerStatus[i] = DISCONNECTED;
	}
}

JoinGameScreen::~JoinGameScreen(void)
{
}

int JoinGameScreen::Update(std::vector<UserCMD>* userCMD, float p_dt)
{
	MenuScreen::Update(userCMD,p_dt);
	for (int i = 0; i < 4; i++)
	{
		if (userCMD->at(i).aButtonPressed)
		{
			if (m_playerStatus[i] == CHOOSE_MODELL)
			{
				m_playerStatus[i]=CHOOSE_COLOR;
			}
			else if (m_playerStatus[i] == CHOOSE_COLOR)
			{
				m_playerStatus[i]=READY;
			}
			else if (m_playerStatus[i] == DISCONNECTED)
			{
				m_playerStatus[i]=CHOOSE_MODELL;
			}
		}
		else if (userCMD->at(i).backButtonPressed)
		{
			m_playerStatus[i] = DISCONNECTED;
		}
		if (userCMD->at(i).xButtonPressed && (m_playerStatus[i] == CHOOSE_MODELL || m_playerStatus[i] == CHOOSE_COLOR))
		{
			m_playerStatus[i] = READY;
		}
		if (userCMD->at(i).bButtonPressed)
		{
			if (m_playerStatus[i] == READY)
			{
				m_playerStatus[i]=CHOOSE_COLOR;
			}
			else if (m_playerStatus[i] == CHOOSE_COLOR)
			{
				m_playerStatus[i]=CHOOSE_MODELL;
			}
			else if (m_playerStatus[i] == CHOOSE_MODELL)
			{
				return GAME_SETUP_SCREEN;
			}	
		}

		if (m_playerStatus[i]==CHOOSE_MODELL)
		{
			if (userCMD->at(i).Joystick.x < -0.8)
			{
				if (m_modell[i] > 0)
				{
					m_modell[i]--;
				}
				
			}
			else if (userCMD->at(i).Joystick.x > 0.8)
			{
				if (m_modell[i] < NUMBER_OF_MODELLS - 1)
				{
					m_modell[i]++;
				}	
			}
		}
		if (m_playerStatus[i]==CHOOSE_COLOR)
		{
			if (userCMD->at(i).Joystick.x < -0.8)
			{
				if (m_color[i] > 0)
				{
					m_color[i]--;
				}
				
			}
			else if (userCMD->at(i).Joystick.x > 0.8)
			{
				if (m_color[i] < NUMBER_OF_COLORS - 1)
				{
					m_color[i]++;
				}	
			}
		}

		if (userCMD->at(i).startButtonPressed)
		{
			int t_numberOfPlayersReady = 0;
			int t_numberOfPlayersDisconnected = 0;
			for (int i = 0; i < 4; i++)
			{
				if (m_playerStatus[i]== READY)
				{
					t_numberOfPlayersReady++;
				}
				else if (m_playerStatus[i]== DISCONNECTED)
				{
					t_numberOfPlayersReady++;
					t_numberOfPlayersDisconnected++;
				}
				else
				{
					break;
				}
			}
			if (t_numberOfPlayersReady == 4 && t_numberOfPlayersDisconnected != 4)
			{
				SaveInfo();
				return GAME_SCREEN;
			}
		}
	}

	return JOIN_GAME_SCREEN;
}
void JoinGameScreen::Draw()
{

}

void JoinGameScreen::SaveInfo()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_playerStatus[i] != DISCONNECTED)
		{
			m_gameInfo->playerOnline[i] = true;
		}
		m_gameInfo->shipModell[i] = m_modell[i];
		m_gameInfo->playerColor[i] = m_color[i];
	}
}