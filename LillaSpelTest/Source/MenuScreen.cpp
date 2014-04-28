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

int MenuScreen::Update(std::vector<UserCMD>* p_userCMDs, float p_dt)
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

std::string MenuScreen::NavigateMenu(UserCMD& p_userCMD)
{


	if (timeSinceLastChange[0] < 0.5)
	{
		if (p_userCMD.aButtonPressed)
		{
			timeSinceLastChange[0]=0;
			return currentButton->buttonName;
		}

		if (p_userCMD.Joystick.y > 0.8 && currentButton->upButton != nullptr)
		{
			currentButton = currentButton->upButton;
		}
		else if (p_userCMD.Joystick.y < -0.8 && currentButton->downButton != nullptr)
		{
			currentButton = currentButton->downButton;
		}
		else if (p_userCMD.Joystick.x > 0.8 && currentButton->rightButton != nullptr)
		{
			currentButton = currentButton->rightButton;
		}
		else if (p_userCMD.Joystick.x < -0.8 && currentButton->leftButton != nullptr)
		{
			currentButton = currentButton->leftButton;
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

	unsigned int t_t1 = 35;
	unsigned int t_t2 = 35;
	m_graphicHandle->LoadTexture(texture1,t_t1);
	m_graphicHandle->LoadTexture(texture2,t_t2);
	m_graphicHandle->CreateHUDObject(buttonList[0]->centerPoint,buttonList[0]->offset,t_t1,t_t2,buttonList[0]->buttonHandle);
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
