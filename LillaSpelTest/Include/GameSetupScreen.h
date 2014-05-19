#pragma once
#include "menuscreen.h"
class GameSetupScreen :
	public MenuScreen
{
private:
	int m_numberOfMaps;
	int currentMap;
	unsigned int m_hudHandle;
public:
	GameSetupScreen(void);
	GameSetupScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager, std::vector<std::string> p_mapNames);
	~GameSetupScreen(void);

	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
private:
	void SaveInfo();
};

