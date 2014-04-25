#include "OptionsScreen.h"


OptionsScreen::OptionsScreen(void)
{
}

OptionsScreen::OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle)
{
	AddButton("Volume", DirectX::XMFLOAT2(0,0.8),0.7,0.1);
	AddButton("Volume", DirectX::XMFLOAT2(0,0.8),0.7,0.1);
}


OptionsScreen::~OptionsScreen(void)
{
}
