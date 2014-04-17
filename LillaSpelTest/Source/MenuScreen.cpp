#include "MenuScreen.h"


MenuScreen::MenuScreen(void)
{
}


MenuScreen::~MenuScreen(void)
{
}

MenuScreen::MenuScreen(GameInfo* p_gameInfo)
{
	m_gameInfo = p_gameInfo;
}

int MenuScreen::Update(std::vector<UserCMD>* p_userCMDs){ return 1;}
void MenuScreen::Draw(){}

void MenuScreen::NavigateMenu(UserCMD* p_userCMD){}

void MenuScreen::AddButton(std::string p_buttonName, DirectX::XMFLOAT2 p_centerPoint, float offsetX, float offsetY)
{
	Button t_button;
	t_button.centerPoint = p_centerPoint;
	t_button.upperLeftCorner = DirectX::XMFLOAT2(p_centerPoint.x-offsetX, p_centerPoint.y + offsetY);
	t_button.lowerRightCorner = DirectX::XMFLOAT2(p_centerPoint.x+offsetX, p_centerPoint.y - offsetY);
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
				//DirectX::XMFLOAT2 t_vecBetweenCenterPoints = DirectX::XMFLOAT2(buttonList[i].centerPoint.x - buttonList[j].centerPoint.x,buttonList[i].centerPoint.y - buttonList[j].centerPoint.y);
				//float t_degrees = 
				//if ()
				//{

				//}

				if (t_buttonToFixCenter.x < t_buttonToCheckCenter.x+0.1f && t_buttonToFixCenter.x > t_buttonToCheckCenter.x-0.1f)
				{
					if (t_buttonToFixCenter.y < t_buttonToCheckCenter.y)
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
							buttonList[i]->downButton = buttonList[j];
						}
					}
				}
				else
				{

				}
			}
		}
	}
}
