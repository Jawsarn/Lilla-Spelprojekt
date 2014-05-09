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
	GameScreen(int p_colors[4], int p_whatVehicle[4], string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle, AudioManager* p_audioManager);
	~GameScreen(void);
	int Update(float p_dt, std::vector<UserCMD>* p_userCMDS); //shouldnt be here
	void PreUpdate(float p_dt, std::vector<UserCMD>* p_userCMDS, int p_Player);
	
private:

	//big methods

	int PauseCheck(int p_currentPlayer, UserCMD p_userCmd);
	int UpdatePlayer(int p_currentPlayer, float p_dt, UserCMD p_userCmd);
	void PlacePlayerWall(int p_currentPlayer);
	void CollisionCheck(int p_currentPlayer, float p_dt,UserCMD& p_userCMD);
	void UpdatePlayerRacePosition(int p_currentPlayer);
	void DrawPlayerHUD(int p_player);
	void DrawPlayer(int p_currentPlayer);

	//help methods
	void PlayerDieStaticObj(int p_currentPlayer);
	void PlayerDiePlayerWall(int p_currentPlayer);
	void PlayerCloseToWall(int p_currentPlayer, int p_wallsCloseTo, float p_dt);

	void CreatePlayerHUDs(int p_numberOfPlayers,int p_color[4]);

	void CountDownHudUpdate(float p_dt);


	//unused
		void Draw();
};

