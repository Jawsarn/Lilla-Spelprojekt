#include "MenuScreen.h"


MenuScreen::MenuScreen(void)
{
	for (int i = 0; i < 4; i++)
	{
		timeSinceLastChange[i]=0;
	}
}


MenuScreen::~MenuScreen(void)
{
}

MenuScreen::MenuScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager)
	:Screen(p_graphicsHandle, p_audioManager)
{
	m_buttonOffset = DirectX::XMFLOAT2(0.5,0.2);
	m_gameInfo = p_gameInfo;
	for (int i = 0; i < 4; i++)
	{
		timeSinceLastChange[i]=0;
	}
}

int MenuScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDs)
{
	for (int i = 0; i < 4; i++)
	{
		timeSinceLastChange[i] += p_dt;
	}
	return 1;
}
void MenuScreen::Draw(){}

void MenuScreen::Initialize()
{
	m_audioManager->PlaySpecificSound("menu.mp3",true,AUDIO_ONLY_PLAY_ONE);
	for (int i = 0; i < 4; i++)
	{
		timeSinceLastChange[i] = 0;
	}
}

bool MenuScreen::FloatBetweenValues(float floatToCheck, float min, float max)
{
	return floatToCheck<max && floatToCheck>min;
}

std::string MenuScreen::NavigateMenu(UserCMD& p_userCMD, unsigned int p_hudHandle)
{


	if (timeSinceLastChange[0] > 0.2)
	{
		if (p_userCMD.aButtonPressed)
		{
			timeSinceLastChange[0]=0;
			return currentButton->buttonName;
		}

		if (p_userCMD.Joystick.y > 0.8 && currentButton->upButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 0);
			currentButton = currentButton->upButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 1);
		}
		else if (p_userCMD.Joystick.y < -0.8 && currentButton->downButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 0);
			currentButton = currentButton->downButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 1);
		}
		else if (p_userCMD.Joystick.x > 0.8 && currentButton->rightButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 0);
			currentButton = currentButton->rightButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 1);
		}
		else if (p_userCMD.Joystick.x < -0.8 && currentButton->leftButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 0);
			currentButton = currentButton->leftButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, 1);
		}
		else
		{
			return ""; 
		}
		m_audioManager->PlaySpecificSound("buttonChange.wav", false,AUDIO_STOP_AND_PLAY);
		timeSinceLastChange[0]=0;
	}
	return ""; 
}

void MenuScreen::AddButton(std::string p_buttonName, DirectX::XMFLOAT2 p_centerPoint, float offsetX, float offsetY,const wchar_t* texture1, const wchar_t* texture2) 
{
	Button t_button;
	t_button.centerPoint = p_centerPoint;
	t_button.offset = m_buttonOffset;
	t_button.buttonName = p_buttonName;
	buttonList.push_back(new Button(t_button));
	int t_index = buttonList.size()-1;
	unsigned int t_t1 = 3555;
	unsigned int t_t2 = 3555;
	m_graphicHandle->LoadTexture(texture1,t_t1);
	m_graphicHandle->LoadTexture(texture2,t_t2);
	
	if (t_t1 == 3555 || t_t2 == 3555)
	{
		//exit(3555); // Failed to load a button texture
	}
	std::vector<unsigned int> t_textureIDs;
	t_textureIDs.push_back(t_t1);
	t_textureIDs.push_back(t_t2);
	m_graphicHandle->CreateHUDObject(buttonList[t_index]->centerPoint,buttonList[t_index]->offset,t_textureIDs,buttonList[t_index]->buttonHandle);
}

void MenuScreen::MakeHud(unsigned int & o_hudID)
{
	std::vector<unsigned int> t_buttonHandles;
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	for (int i = 0; i < buttonList.size(); i++)
	{
		t_buttonHandles.push_back(buttonList[i]->buttonHandle);
		t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
		buttonList[i]->buttonHandle = i;
	}
	unsigned int t_templateHandle;
	m_graphicHandle->CreateHudTemplate(t_buttonHandles,t_templateHandle);
	m_graphicHandle->CreateHudFromTemplate(t_templateHandle,5,t_barOffsets,o_hudID);
}

void MenuScreen::FixButtonPointers()
{
	int t_numberOfButtons = buttonList.size();
	DirectX::XMFLOAT2 horizontal = DirectX::XMFLOAT2(1,0);
	DirectX::XMFLOAT2 vertical = DirectX::XMFLOAT2(0,1);
	for (int i = 0; i < t_numberOfButtons; i++)
	{
		DirectX::XMFLOAT2 t_buttonToFixCenter = buttonList[i]->centerPoint;
		for (int j = 0; j < t_numberOfButtons; j++)
		{
			
			if (i != j )
			{
				DirectX::XMFLOAT2 t_buttonToCheckCenter = buttonList[j]->centerPoint;

				if (t_buttonToFixCenter.x < t_buttonToCheckCenter.x+0.05f && t_buttonToFixCenter.x > t_buttonToCheckCenter.x-0.05f)
				{
					if (t_buttonToFixCenter.y > t_buttonToCheckCenter.y)
					{
						if (buttonList[i]->downButton == nullptr)
						{
							buttonList[i]->downButton = buttonList[j];
						}
						else if (CheckIfButtonIsClosest(t_buttonToFixCenter,t_buttonToCheckCenter,buttonList[i]->downButton))
						{
							buttonList[i]->downButton = buttonList[j];
						}
					}
					else
					{
						if (buttonList[i]->upButton == nullptr)
						{
							buttonList[i]->upButton = buttonList[j];
						}
							else if (CheckIfButtonIsClosest(t_buttonToFixCenter,t_buttonToCheckCenter,buttonList[i]->upButton))
						{
							buttonList[i]->upButton = buttonList[j];
						}
					}
				}
				else
				{
					if (t_buttonToFixCenter.x > t_buttonToCheckCenter.x)
					{
						if (buttonList[i]->leftButton == nullptr)
						{
							buttonList[i]->leftButton = buttonList[j];
						}
					
						else if (CheckIfButtonIsClosest(t_buttonToFixCenter,t_buttonToCheckCenter,buttonList[i]->leftButton))
						{						
							buttonList[i]->leftButton = buttonList[j];										
						}
					}
					else
					{
						if (buttonList[i]->rightButton == nullptr)
						{
							buttonList[i]->rightButton = buttonList[j];
						}
						else if (CheckIfButtonIsClosest(t_buttonToFixCenter,t_buttonToCheckCenter,buttonList[i]->rightButton))
						{
							
								buttonList[i]->rightButton = buttonList[j];
							
						}
					}
				}
			}
		}
	}
}

bool MenuScreen::CheckIfButtonIsClosest(DirectX::XMFLOAT2 p_buttonToFixCenter,DirectX::XMFLOAT2 p_buttonToCheckCenter ,Button* p_previousConnectedButton)
{
	bool r_shouldConnect = std::abs(p_buttonToFixCenter.x- p_buttonToCheckCenter.x) < std::abs(p_buttonToFixCenter.x - p_previousConnectedButton->centerPoint.x)|| std::abs(p_buttonToFixCenter.y- p_buttonToCheckCenter.y) < std::abs(p_buttonToFixCenter.y - p_previousConnectedButton->centerPoint.y);
	return r_shouldConnect;
}





