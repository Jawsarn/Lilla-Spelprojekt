#include "MainMenuScreen.h"


MainMenuScreen::MainMenuScreen(void)
	:MenuScreen()
{
	
}

MainMenuScreen::MainMenuScreen(GraphicHandle* p_graphicsHandle)
	:MenuScreen(nullptr,p_graphicsHandle)
{
	AddButton("Start Game", DirectX::XMFLOAT2(0,0.5),0.5,0.2,L"StartGame1.dds",L"StartGame2.dds");
	//AddButton("Options",DirectX::XMFLOAT2(0,0),0.5,0.2,L"Options1.jpg",L"Options2.jpg");
	//AddButton("Quit",DirectX::XMFLOAT2(0,-0.5),0.5,0.2,L"Quit1.jpg",L"Quit2.jpg");

	FixButtonPointers();
	currentButton = buttonList[0];
	std::vector<unsigned int> t_buttonHandles;
	std::vector<DirectX::XMFLOAT2> t_barOffsets;
	for (int i = 0; i < buttonList.size(); i++)
	{
		t_buttonHandles.push_back(buttonList[i]->buttonHandle);
		t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
	}
	unsigned int t_templateHandle;
	m_graphicHandle->CreateHudTemplate(t_buttonHandles,t_templateHandle);
	m_graphicHandle->CreateHudFromTemplate(t_templateHandle,DirectX::XMFLOAT3(0,1,0),t_barOffsets,m_hudHandle);
	//m_graphicHandle->SetViewportAmount(2);
	m_graphicHandle->UseHud(0,m_hudHandle);
}

MainMenuScreen::~MainMenuScreen(void)
{

}

int MainMenuScreen::Update(std::vector<UserCMD>* p_userCMD, float p_dt)
{
	MenuScreen::Update(p_userCMD,p_dt);
	std::string t_menuChoice = NavigateMenu(p_userCMD->at(0));
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

	return MAIN_MENU_SCREEN;
}

void MainMenuScreen::Draw()
{}
