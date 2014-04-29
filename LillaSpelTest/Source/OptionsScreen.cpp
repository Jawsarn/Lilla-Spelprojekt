#include "OptionsScreen.h"


OptionsScreen::OptionsScreen(void)
{
}

OptionsScreen::OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle)
	:MenuScreen(p_gameInfo,p_graphicsHandle)
{
	AddButton("Volume", DirectX::XMFLOAT2(0,0.5),0.7,0.2, L"Volume1.dds",L"Volume2.dds");
	AddButton("Resolution", DirectX::XMFLOAT2(0,0),0.5,0.2,L"Resolution1.dds",L"Resolution2.dds");
	AddButton("Back", DirectX::XMFLOAT2(0,-0.5),0.5,0.2,L"Back1.dds",L"Back2.dds");

	FixButtonPointers();
	MakeHud(m_hudHandle);

	//m_graphicHandle->UseHud(0,m_hudHandle);
	currentButton = buttonList[0];
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle,false);
}


OptionsScreen::~OptionsScreen(void)
{
}

int OptionsScreen::Update(std::vector<UserCMD>* userCMD, float p_dt)
{
	MenuScreen::Update( userCMD, p_dt);
	std::string t_menuChoice = NavigateMenu(userCMD->at(0),m_hudHandle);

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

void OptionsScreen::Draw()
{

}

void OptionsScreen::Initialize()
{
	m_graphicHandle->SetViewportAmount(1);
	m_graphicHandle->UseHud(0,m_hudHandle);

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

