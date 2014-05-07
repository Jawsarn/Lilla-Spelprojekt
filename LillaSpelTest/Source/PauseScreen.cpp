#include "PauseScreen.h"


PauseScreen::PauseScreen(void)
{
}

PauseScreen::PauseScreen(GraphicHandle* p_graphicHandle, AudioManager* p_audioManager)
	:MenuScreen(nullptr,p_graphicHandle, p_audioManager)
{
	AddButton("Continue", DirectX::XMFLOAT2(0,0.5),0.5,0.2,L"continue1.dds",L"continue2.dds");
	AddButton("Main Menu",DirectX::XMFLOAT2(0,0),0.5,0.2,L"MainMenu1.dds",L"MainMenu2.dds");
	AddButton("Quit",DirectX::XMFLOAT2(0,-0.5),0.5,0.2,L"Quit1.dds",L"Quit2.dds");

	FixButtonPointers();
	MakeHud(m_hudHandle);
}

PauseScreen::~PauseScreen(void)
{
}

int PauseScreen::Update(float p_dt,std::vector<UserCMD>* userCMD )
{
	MenuScreen::Update(p_dt,userCMD);
	std::string t_menuChoice = NavigateMenu(userCMD->at(m_whoPaused),m_hudHandle);
	if (t_menuChoice == "Continue")
	{
		return GAME_SCREEN;
	}
	else if (t_menuChoice == "Main Menu")
	{
		return MAIN_MENU_SCREEN;
	}
	else if (t_menuChoice == "Quit")
	{
		return SHUT_DOWN;
	}
	return PAUSE_SCREEN;
}
void PauseScreen::Draw()	
{

}
void PauseScreen::Initialize(int p_whoPaused)	
{
	m_graphicHandle->UseHud(p_whoPaused,m_hudHandle);
	currentButton = buttonList[0];
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle,1);
	m_whoPaused = p_whoPaused;
}
void PauseScreen::Initialize()	
{

}
