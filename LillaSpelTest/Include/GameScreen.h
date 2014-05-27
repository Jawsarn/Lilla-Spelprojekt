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
	std::string m_tauntSound[4];
	std::string m_engineSound[4];
	float m_vibrationTimer[4];
	int m_lastNodeIndex;
	int m_pauseDudeIndex;
	int m_mapID;
	float m_timeSpentDuringPreUpdate;
	int m_preUpdateCountdown;
	int m_nrOfLaps;
	int m_nrOfFinishedPlayers;
	float m_mapTotalDistance;
	GameScreenState m_state;
	float m_immortalCounter;
public:
	GameScreen(void);
	int GetPauseDudeIndex();
	void Initialize();
	GameScreen(int p_colors[4], int p_whatVehicle[4],string p_tauntSound[4], string p_mapName, int p_numberOfPlayers, GraphicHandle* p_graphicHandle, AudioManager* p_audioManager, int p_nrOfLaps, int p_mapID);
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
	void UpdateSounds(int p_player,std::vector<UserCMD>* p_userCMDS);
	void PlaySounds();
	void StopSounds();
	void UpdateVibration(float p_dt,int p_player, UserCMD& p_userCMD);
	void PlayShockWaveSound();

	//help methods
	void PlayerDieStaticObj(int p_currentPlayer);
	void PlayerDiePlayerWall(int p_currentPlayer);
	void PlayerCloseToWall(int p_currentPlayer, int p_wallsCloseTo, float p_dt);
	void StopAllVibration(std::vector<UserCMD>* p_userCMDS);

	void CreatePlayerHUDs(int p_numberOfPlayers,int p_color[4], std::string p_mapName);

	void CountDownHudUpdate(float p_dt);


	//unused
		void Draw();
};

