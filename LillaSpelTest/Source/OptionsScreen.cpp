#include "OptionsScreen.h"


OptionsScreen::OptionsScreen(void)
{
}

OptionsScreen::OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle)
	:MenuScreen(p_gameInfo,p_graphicsHandle)
{
	AddButton("Volume", DirectX::XMFLOAT2(0,0.8),0.7,0.1, L"Volume1.jpg",L"Volume2.jpg");
	AddButton("Resolution", DirectX::XMFLOAT2(0,0.8),0.5,0.1,L"Resolution1.jpg",L"Resolution2.jpg");
	AddButton("Back", DirectX::XMFLOAT2(0,0.8),0.7,0.1,L"Back1.jpg",L"Back2.jpg");
	FixButtonPointers();
}


OptionsScreen::~OptionsScreen(void)
{
}

int OptionsScreen::Update(std::vector<UserCMD>* userCMD, float p_dt)
{
	MenuScreen::Update( userCMD, p_dt);
	std::string t_menuChoice = NavigateMenu(userCMD->at(0));

	if (currentButton->buttonName == "Volume")
	{
		AlterVolume(userCMD->at(0));
	}
	else if (t_menuChoice == "Resolution")
	{

	}
	else if (t_menuChoice == "Back" || userCMD->at(0).backButtonPressed)
	{
		return MAIN_MENU_SCREEN;
	}

	return OPTIONS_SCREEN;
}

void OptionsScreen::AlterVolume(UserCMD& p_userCMD)
{
	if (timeSinceLastChange[0] <0.03)
	{
		if (p_userCMD.Joystick.x<-0.8 && m_volume > 0)
		{
			m_volume -= VOLUME_CHANGE;
			timeSinceLastChange[0]=0;
		} 
		else if (p_userCMD.Joystick.x>0.8 && m_volume < MAX_VOLUME)
		{
			m_volume += VOLUME_CHANGE;
			timeSinceLastChange[0]=0;
		}
	}
}

