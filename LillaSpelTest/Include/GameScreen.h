#pragma once
#include "screen.h"
#include "Struct_MapNode.h"
#include "Player.h"
#include "MapLoader.h"
#include "CollisionManager.h"
#include <string>

enum GameScreenState{COUNTDOWN, PLAY};

class GameScreen :
	public Screen
{
private:
	MapLoader* m_mapLoader;
	CollisionManager* m_collisionManager;
	std::vector<MapNode*>* m_mapNodes;
	std::vector<Player*> m_players;
	std::vector<UINT> m_hudID;
	int m_lastNodeIndex;
	int m_pauseDudeIndex;
	float m_timeSpentDuringPreUpdate;
	GameScreenState m_state;
public:
	GameScreen(void);
	int GetPauseDudeIndex();
	void Initialize();
	GameScreen(int p_colors[4], int p_whatVehicle[4], string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle);
	~GameScreen(void);
	int Update(float p_dt, std::vector<UserCMD>* p_userCMDS); //shouldnt be here
	void PreUpdate(float p_dt, std::vector<UserCMD>* p_userCMDS, int p_Player);
	void Draw();
private:
	void CreatePlayerHUDs(int p_numberOfPlayers,int p_color[4]);
	void UpdatePlayerHUD(int p_player);
	void CountDownHudUpdate(float p_dt);
};

