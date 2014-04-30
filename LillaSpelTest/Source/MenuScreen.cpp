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

MenuScreen::MenuScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle)
	:Screen(p_graphicsHandle)
{
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

bool MenuScreen::FloatBetweenValues(float floatToCheck, float min, float max)
{
	return floatToCheck<max && floatToCheck>min;
}

std::string MenuScreen::NavigateMenu(UserCMD& p_userCMD, unsigned int p_hudHandle)
{


	if (timeSinceLastChange[0] > 0.5)
	{
		if (p_userCMD.aButtonPressed)
		{
			timeSinceLastChange[0]=0;
			return currentButton->buttonName;
		}

		if (p_userCMD.Joystick.y > 0.8 && currentButton->upButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, true);
			currentButton = currentButton->upButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, false);
		}
		else if (p_userCMD.Joystick.y < -0.8 && currentButton->downButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, true);
			currentButton = currentButton->downButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, false);
		}
		else if (p_userCMD.Joystick.x > 0.8 && currentButton->rightButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, true);
			currentButton = currentButton->rightButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, false);
		}
		else if (p_userCMD.Joystick.x < -0.8 && currentButton->leftButton != nullptr)
		{
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, true);
			currentButton = currentButton->leftButton;
			m_graphicHandle->ChangeHudObjectTexture(p_hudHandle,currentButton->buttonHandle, false);
		}
		else
		{
			return ""; 
		}
		timeSinceLastChange[0]=0;
	}
	return ""; 
}

void MenuScreen::AddButton(std::string p_buttonName, DirectX::XMFLOAT2 p_centerPoint, float offsetX, float offsetY,const wchar_t* texture1, const wchar_t* texture2) 
{
	Button t_button;
	t_button.centerPoint = p_centerPoint;
	t_button.offset = DirectX::XMFLOAT2(offsetX,offsetY);
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
	m_graphicHandle->CreateHUDObject(buttonList[t_index]->centerPoint,buttonList[t_index]->offset,t_t1,t_t2,buttonList[t_index]->buttonHandle);
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
	m_graphicHandle->CreateHudFromTemplate(t_templateHandle,DirectX::XMFLOAT3(0,1,0),t_barOffsets,o_hudID);
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

				if (t_buttonToFixCenter.x < t_buttonToCheckCenter.x+0.1f && t_buttonToFixCenter.x > t_buttonToCheckCenter.x-0.1f)
				{
					if (t_buttonToFixCenter.y > t_buttonToCheckCenter.y)
					{
						if (buttonList[i]->downButton == nullptr)
						{
							buttonList[i]->downButton = buttonList[j];
						}
						else if (std::abs(t_buttonToFixCenter.y- t_buttonToCheckCenter.y) < std::abs(t_buttonToFixCenter.y - buttonList[i]->downButton->centerPoint.y))
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
							else if (std::abs(t_buttonToFixCenter.y- t_buttonToCheckCenter.y) < std::abs(t_buttonToFixCenter.y - buttonList[i]->upButton->centerPoint.y))
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
					
						else if (std::abs(t_buttonToFixCenter.x- t_buttonToCheckCenter.x) < std::abs(t_buttonToFixCenter.x - buttonList[i]->leftButton->centerPoint.x))
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
							else if (std::abs(t_buttonToFixCenter.x- t_buttonToCheckCenter.x) < std::abs(t_buttonToFixCenter.x - buttonList[i]->rightButton->centerPoint.x))
						{
							buttonList[i]->rightButton = buttonList[j];
						}
					}
				}
			}
		}
	}
}
