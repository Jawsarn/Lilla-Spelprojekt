#pragma once
#include "screen.h"
#include "Struct_MapNode.h"
#include "Player.h"
#include "MapLoader.h"
#include "CollisionManager.h"
#include <string>

class GameScreen :
	public Screen
{
private:
	MapLoader* m_mapLoader;
	CollisionManager* m_collisionManager;
	std::vector<MapNode*>* m_mapNodes;
	std::vector<Player*> m_players;
public:
	GameScreen(void);
	GameScreen(std::string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle);
	~GameScreen(void);
	int Update(float p_dt, std::vector<UserCMD>* p_userCMDS); //shouldnt be here
	void Draw();
};

