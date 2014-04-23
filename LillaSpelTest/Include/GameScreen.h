#pragma once
#include "screen.h"
#include "MapNode.h"
#include "Player.h"
#include "MapLoader.h"
#include <string>
#include "GraphicHandle.h"

class GameScreen :
	public Screen
{
private:
	MapLoader* mapLoader;
	std::vector<MapNode*> m_mapNodes;
	std::vector<Player*> m_players;
public:
	GameScreen(void);
	GameScreen(std::string p_mapName, int p_numberOfPlayers );
	~GameScreen(void);
	int Update(float p_dt, std::vector<UserCMD>* p_userCMDS, GraphicHandle* m_GraphicHandle); //shouldnt be here
	void Draw();
};

