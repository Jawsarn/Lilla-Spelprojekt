#pragma once
#include "menuscreen.h"
class GameSetupScreen :
	public MenuScreen
{
private:
	std::vector<std::string> mapList;
	int currentMap;
public:
	GameSetupScreen(void);
	GameSetupScreen(GameInfo* p_gameInfo);
	~GameSetupScreen(void);

	int Update(std::vector<UserCMD>* userCMD, float p_dt);
	void Draw();
private:
	void SaveInfo();
};

