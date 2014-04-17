#pragma once
#include "menuscreen.h"
class GameSetupScreen :
	public MenuScreen
{
private:
	std::string currentMap;
	std::vector<std::string> mapList;
public:
	GameSetupScreen(void);
	GameSetupScreen(GameInfo* p_gameInfo);
	~GameSetupScreen(void);

	int Update(std::vector<UserCMD>* userCMD);
	void Draw();
};

