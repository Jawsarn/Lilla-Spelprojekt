#pragma once
#include "menuscreen.h"
class GameSetupScreen :
	public MenuScreen
{
private:
	int m_numberOfMaps;
	int currentMap;
	unsigned int m_hudHandle;
	int m_numberOfLaps;
	int m_maxLaps;
	std::vector<bool> m_levelMultipleLaps;
public:
	GameSetupScreen(void);
	GameSetupScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager, std::vector<std::string> p_mapNames,std::vector<bool> p_levelMultipleLaps);
	~GameSetupScreen(void);

	int Update(float p_dt,std::vector<UserCMD>* userCMD );
	void Draw();
	void Initialize();
private:
	void SaveInfo();
	void ChangeLevel(std::vector<UserCMD>* userCMD );
	void ChangeNumberOfLaps(std::vector<UserCMD>* userCMD );
};

