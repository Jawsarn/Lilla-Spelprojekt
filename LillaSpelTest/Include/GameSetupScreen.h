#pragma once
#include "menuscreen.h"
class GameSetupScreen :
	public MenuScreen
{
private:
	std::vector<std::string> mapList;
	int currentMap;
	unsigned int m_hudHandle;
public:
	GameSetupScreen(void);
	GameSetupScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle);
	~GameSetupScreen(void);

	int Update(std::vector<UserCMD>* userCMD, float p_dt);
	void Draw();
	void Initialize();
private:
	void SaveInfo();
};

