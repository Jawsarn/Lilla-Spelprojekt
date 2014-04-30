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

	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
private:
	void SaveInfo();
};

