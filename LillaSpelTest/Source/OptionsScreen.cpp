#include "OptionsScreen.h"


OptionsScreen::OptionsScreen(void)
{
}

OptionsScreen::OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager)
	:MenuScreen(p_gameInfo,p_graphicsHandle,  p_audioManager)
{
	m_volume = m_audioManager->GetMasterVolume();

	AddButton("Volume", DirectX::XMFLOAT2(0,0.6),0.5,1, L"Volume1.dds",L"Volume2.dds");
	AddButton("Resolution", DirectX::XMFLOAT2(0,0.2),0.5,1,L"Resolution1.dds",L"Resolution2.dds");
	AddButton("Full screen",DirectX::XMFLOAT2(0,-0.2),0.5,1,L"fullScreen1.dds",L"FullScreen2.dds");
	AddButton("Back", DirectX::XMFLOAT2(0,-0.6),0.5,0.2,L"Back1.dds",L"Back2.dds");
	std::vector<UINT> t_volumeSlideTextures;
	UINT t_volumeSlideTexture;
	UINT t_volumeSlideHandle;
	m_graphicHandle->LoadTexture(L"VolumeSlide.dds",t_volumeSlideTexture);
	t_volumeSlideTextures.push_back(t_volumeSlideTexture);
	m_graphicHandle->CreateHUDObject(DirectX::XMFLOAT2(0,0.85),DirectX::XMFLOAT2(0.5,0.07),t_volumeSlideTextures,t_volumeSlideHandle);


	FixButtonPointers();
	//MakeHud(m_hudHandle);

	std::vector<unsigned int> t_buttonHandles;
	std::vector<DirectX::XMFLOAT2> t_barOffsets;

	for (int i = 0; i < buttonList.size(); i++)
	{
		t_buttonHandles.push_back(buttonList[i]->buttonHandle);
		t_barOffsets.push_back(DirectX::XMFLOAT2(0,0));
		buttonList[i]->buttonHandle = i;
	}
	t_buttonHandles.push_back(t_volumeSlideHandle);
	t_barOffsets.push_back(DirectX::XMFLOAT2(1-m_volume,0));
	m_volumeSlideHandle = t_barOffsets.size() - 1;

	unsigned int t_templateHandle;
	m_graphicHandle->CreateHudTemplate(t_buttonHandles,t_templateHandle);
	m_graphicHandle->CreateHudFromTemplate(t_templateHandle,5,t_barOffsets,m_hudHandle);

	//m_graphicHandle->UseHud(0,m_hudHandle);
	currentButton = buttonList[0];
	
	m_fullScreen=false;
	
}


OptionsScreen::~OptionsScreen(void)
{
}

int OptionsScreen::Update(float p_dt,std::vector<UserCMD>* userCMD )
{
	MenuScreen::Update( p_dt,userCMD );
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
	else if (t_menuChoice == "Full screen")
	{
		if (m_fullScreen == true) //fullscreen dosent work with breakpoints
		{
			m_graphicHandle->SetFullScreen(false);
			m_fullScreen = false;
		}
		else
		{
			m_graphicHandle->SetFullScreen(true);
			m_fullScreen = true;
		}
		
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
	m_graphicHandle->ChangeHudObjectTexture(m_hudHandle,currentButton->buttonHandle,1);
	MenuScreen::Initialize();
}


void OptionsScreen::AlterVolume(UserCMD& p_userCMD)
{
	if (timeSinceLastChange[0] >0.03)
	{
		if (p_userCMD.Joystick.x<-0.8 && m_volume > 0)
		{
			m_volume -= VOLUME_CHANGE;
			timeSinceLastChange[0]=0;
			m_audioManager->SetMasterVolume(m_volume);
			m_graphicHandle->UpdateHudBarOffset(m_hudHandle,m_volumeSlideHandle,DirectX::XMFLOAT2(1*0.83-m_volume*0.83,0));
		} 
		else if (p_userCMD.Joystick.x>0.8 && m_volume < MAX_VOLUME)
		{
			m_volume += VOLUME_CHANGE;
			timeSinceLastChange[0]=0;
			m_audioManager->SetMasterVolume(m_volume);
			m_graphicHandle->UpdateHudBarOffset(m_hudHandle,m_volumeSlideHandle,DirectX::XMFLOAT2(1*0.83-m_volume*0.83,0));
		}
		
	}
}

