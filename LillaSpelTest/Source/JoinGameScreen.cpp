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

JoinGameScreen::JoinGameScreen(GameInfo* p_gameInfo,GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager)
	:MenuScreen(p_gameInfo,p_graphicsHandle, p_audioManager)
{
	numberOfModells = m_graphicHandle->GetAmountOfVehicles();
	for (int i = 0; i < 4; i++)
	{
		m_modell[i] = 0;
		m_color[i] = 0;
		m_playerStatus[i] = DISCONNECTED;
		m_modellIncrease[i] = 0;
	}

	MakeHud(L"Connect.dds", DISCONNECTED);
	MakeHud(L"ChooseModellButton.dds",CHOOSE_MODELL);
	MakeHud(L"ChooseColorButton.dds",CHOOSE_COLOR);
	MakeHud(L"StartToStartButton.dds",READY);
	
}

JoinGameScreen::~JoinGameScreen(void)
{
}

int JoinGameScreen::Update(float p_dt,std::vector<UserCMD>* userCMD)
{
	MenuScreen::Update(p_dt,userCMD);
	m_graphicHandle->UpdateSelectVehicle(p_dt);
	for (int i = 0; i < 4; i++)
	{
		if (timeSinceLastChange[i]>0.1)
		{
			if (userCMD->at(i).aButtonPressed)
			{
				timeSinceLastChange[i] = 0;
				if (m_playerStatus[i] == CHOOSE_MODELL)
				{
					m_playerStatus[i]=CHOOSE_COLOR;
					m_graphicHandle->UseHud(i,m_hudIDs[CHOOSE_COLOR]);
				}
				else if (m_playerStatus[i] == CHOOSE_COLOR)
				{
					m_playerStatus[i]=READY;
					m_graphicHandle->UseHud(i,m_hudIDs[READY]);
				}
				else if (m_playerStatus[i] == DISCONNECTED)
				{
					m_playerStatus[i]=CHOOSE_MODELL;
					m_graphicHandle->UseHud(i,m_hudIDs[CHOOSE_MODELL]);
				}
			}
			else if (userCMD->at(i).backButtonPressed)
			{
				timeSinceLastChange[i] = 0;
				m_graphicHandle->RemoveSelectionDraw();
				return GAME_SETUP_SCREEN;
			}
			if (userCMD->at(i).xButtonPressed && (m_playerStatus[i] == CHOOSE_MODELL || m_playerStatus[i] == CHOOSE_COLOR))
			{
				timeSinceLastChange[i] = 0;
				m_playerStatus[i] = READY;
			}
			if (userCMD->at(i).bButtonPressed)
			{
				timeSinceLastChange[i] = 0;
				if (m_playerStatus[i] == READY)
				{
					m_playerStatus[i]=CHOOSE_COLOR;
					m_graphicHandle->UseHud(i,m_hudIDs[CHOOSE_COLOR]);
				}
				else if (m_playerStatus[i] == CHOOSE_COLOR)
				{
					m_graphicHandle->UseHud(i,m_hudIDs[CHOOSE_MODELL]);
					m_playerStatus[i]=CHOOSE_MODELL;
				}
				else if (m_playerStatus[i] == CHOOSE_MODELL)
				{
					m_graphicHandle->SetCameraVehicleSelection(i);
					m_graphicHandle->UseHud(i,m_hudIDs[DISCONNECTED]);
					m_playerStatus[i] = DISCONNECTED;
				}
			}

			if (userCMD->at(i).startButtonPressed)
			{
				timeSinceLastChange[i] = 0;
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
		if (m_playerStatus[i]==CHOOSE_MODELL)
		{
			ModellChanger(i,p_dt,userCMD);
		}
		if (m_playerStatus[i]==CHOOSE_COLOR)
		{
			ColorChanger(i,p_dt,userCMD);
		}
	}

	return JOIN_GAME_SCREEN;
}
void JoinGameScreen::Draw()
{
}

void JoinGameScreen::Initialize()
{
	for (int i = 0; i < 4; i++)
	{
		m_graphicHandle->SetCameraVehicleSelection(i);
		m_graphicHandle->UseHud(i,m_hudIDs[DISCONNECTED]);
		m_playerStatus[i]=DISCONNECTED;
	}
	m_graphicHandle->AddSelectionDraw();
	m_graphicHandle ->SetViewportAmount(4);
	
}

void JoinGameScreen::MakeHud(const wchar_t* p_textureNames, int p_hudIndex)
{
	DirectX::XMFLOAT2 t_centerPoint = DirectX::XMFLOAT2(0,-0.8);
	DirectX::XMFLOAT2 t_offset = DirectX::XMFLOAT2(0.3,0.1);
	std::vector<unsigned int> t_objHandles;
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	
	unsigned int t_t1;
	unsigned int t_objHandle;
	unsigned int t_templateHandle;
	m_graphicHandle->LoadTexture(p_textureNames,t_t1);
	std::vector<unsigned int> t_textureIDs;
	t_textureIDs.push_back(t_t1);
	m_graphicHandle->CreateHUDObject(t_centerPoint,t_offset,t_textureIDs,t_objHandle);
	t_objHandles.push_back(t_objHandle);
	m_graphicHandle->CreateHudTemplate(t_objHandles,t_templateHandle);
	m_graphicHandle->CreateHudFromTemplate(t_templateHandle,5,t_barOffsets,m_hudIDs[p_hudIndex]);
}

void JoinGameScreen::SaveInfo()
{
	m_graphicHandle->RemoveSelectionDraw();
	m_graphicHandle->AddLevelDraw(m_gameInfo->map);
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

void JoinGameScreen::ModellChanger(int i, float p_dt, std::vector<UserCMD>* userCMD)
{
	if (timeSinceLastChange[i] < 0.5 && m_modellIncrease[i]!=0)
	{
		if (m_modellIncrease[i] == 1)
		{
			m_graphicHandle->UpdateCameraVehicleSelection(i,(m_modell[i]-1)+timeSinceLastChange[i]*2);
		}
		else if (m_modellIncrease[i] == -1)
		{
			m_graphicHandle->UpdateCameraVehicleSelection(i,(m_modell[i]+1)-timeSinceLastChange[i]*2);
		}			
	}
	else if (timeSinceLastChange[i] < 1.2)
	{
		m_graphicHandle->UpdateCameraVehicleSelection(i,m_modell[i]);
		m_modellIncrease[i]=0;
	}
	else if (userCMD->at(i).Joystick.x < -0.8)
	{
		timeSinceLastChange[i]=0;
		m_modellIncrease[i] = -1;
		if (m_modell[i] > 0)
		{
			m_modell[i]--;
		}
		else
		{
			m_modell[i] = numberOfModells - 1;
		}
	}
	else if (userCMD->at(i).Joystick.x > 0.8)
	{
		timeSinceLastChange[i]=0;
		m_modellIncrease[i] = 1;
		if (m_modell[i] < numberOfModells - 1)
		{
			m_modell[i]++;
		}
		else
		{
			m_modell[i] = 0;
		}
	}
}
void JoinGameScreen::ColorChanger(int i, float p_dt, std::vector<UserCMD>* userCMD)
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