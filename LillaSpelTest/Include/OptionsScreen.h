#pragma once
#include "menuscreen.h"
class OptionsScreen :
	public MenuScreen
{
public:
	OptionsScreen(void);
	OptionsScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle);
	~OptionsScreen(void);
};

