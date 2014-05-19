#include "MainMenuScreen.h"


MainMenuScreen::MainMenuScreen(void)
	:MenuScreen()
{
	
}

MainMenuScreen::MainMenuScreen(GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager)
	:MenuScreen(nullptr,p_graphicsHandle,p_audioManager)
{
	//Menu
	AddButton("Start Game", DirectX::XMFLOAT2(0,0.5),0.5,1,L"startgame1.dds",L"StartGame2.dds");
	AddButton("Options",DirectX::XMFLOAT2(0,0),0.5,1,L"Options1.dds",L"Options2.dds");
	AddButton("Quit",DirectX::XMFLOAT2(0,-0.5),0.5,1,L"Quit1.dds",L"Quit2.dds");
	//AddButton("Debugg", DirectX::XMFLOAT2(-0.7,0),0.1,0.1,L"Debugg1.dds",L"Debugg2.dds");

	FixButtonPointers();
	MakeHud(m_hudHandle);

	m_graphicHandle->SetViewportAmount(1);
	m_graphicHandle->UseHud(0,m_hudHandle);
	currentButton = buttonList[0];
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle,1);
	
}

MainMenuScreen::~MainMenuScreen(void)
{

}

int MainMenuScreen::Update(float p_dt,std::vector<UserCMD>* userCMD )
{
	MenuScreen::Update(p_dt,userCMD);
	std::string t_menuChoice = NavigateMenu(userCMD->at(0),m_hudHandle);
	if (t_menuChoice == "Start Game")
	{
		return GAME_SETUP_SCREEN;
	}
	else if (t_menuChoice == "Options")
	{
		return OPTIONS_SCREEN;
	}
	else if (t_menuChoice == "Quit")
	{
		return SHUT_DOWN;
	}
	else if (t_menuChoice == "Debugg")
	{
		//return DEBUGG;
	}

	return MAIN_MENU_SCREEN;
}

void MainMenuScreen::Draw()
{}

void MainMenuScreen::Initialize()
{
	m_graphicHandle->SetViewportAmount(1);
	m_graphicHandle->UseHud(0,m_hudHandle);
	MenuScreen::Initialize();
}
