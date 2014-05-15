#include "GoalScreen.h"


GoalScreen::GoalScreen(void)
{
}

GoalScreen::GoalScreen( GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager)
	:MenuScreen(nullptr,p_graphicsHandle,p_audioManager)
{
	AddButton("Restart",DirectX::XMFLOAT2(0,0.5),0,0,L"Restart1.dds",L"Restart2.dds");
	AddButton("Main Menu",DirectX::XMFLOAT2(0,0),0,0,L"MainMenu1.dds",L"MainMenu2.dds");
	AddButton("Quit",DirectX::XMFLOAT2(0,-0.5),0,0,L"Quit1.dds",L"Quit2.dds");

	FixButtonPointers();
	MakeHud(m_hudHandle);
	currentButton=buttonList[0];
}

GoalScreen::~GoalScreen(void)
{
}

int GoalScreen::Update(float p_dt, std::vector<UserCMD>* p_userCMDs)
{
	MenuScreen::Update(p_dt,p_userCMDs);
	std::string t_menuChoice = NavigateMenu(p_userCMDs->at(0),m_hudHandle);

	if (t_menuChoice == "Restart")
	{
		return GAME_SCREEN;
	}
	else if (t_menuChoice == "Main menu")
	{
		return MAIN_MENU_SCREEN;
	}
	else if (t_menuChoice == "Quit")
	{
		return SHUT_DOWN;
	}
	return GOAL_SCREEN;
}
void GoalScreen::Draw()
{

}
void GoalScreen::Initialize()
{

	m_graphicHandle->SetViewportAmount(1);
	m_graphicHandle->UseHud(0,m_hudHandle);
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle, 0);
	currentButton=buttonList[0];
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle, 1);

	MenuScreen::Initialize();
}
