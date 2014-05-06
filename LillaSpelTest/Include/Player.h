#pragma once
#include "object.h"
#include "PlayerWall.h"
#include "Struct_MapNode.h"
#include "PlayerWall.h"
#include "UserCMD.h"
#include <DirectXCollision.h>

using namespace DirectX;

enum PlayerState{NORMAL,DEAD,IMMORTAL};

#define DEATH_TIME 2
#define IMMORTAL_TIME 2

class Player :
	public Object
{
	///////////////VARIABLES///////////
private:
	//speed stuff
	float m_speed;
	float m_maxSpeed;
	float m_maxBoostSpeed;
	float m_acceleration;
	float m_boostAcceleration;
	float m_deceleration;

	//rotation stuff
	float m_rotateSpeed;
	float m_angle;

	//boost stuff
	float m_boostDecay;
	float m_maxBoost;
	float m_boostMeter;

	//wall stuff
	float m_wallMeter;
	float m_maxWalls;
	float m_coolDown;
	float m_coolDownDecay;

	float m_distance;



	///Timers///
	float m_deathTimer;
	float m_maxDeathTimer;
	float m_immortalTimer;
	float m_maxImmortalTimer;
	int m_racePos;
	float m_wallGain;

	XMFLOAT3 m_logicalPosition;
	//XMFLOAT3 m_acceleration;
	XMFLOAT3 m_color;
	XMFLOAT3 m_upVector;

	XMFLOAT3 m_wallBoxExtents;
	XMFLOAT3 m_playerShipBoxExtents;

	XMFLOAT4X4 m_cameraMatrix;
	XMFLOAT4X4 m_worldMatrix;
	PlayerWall* m_lastPlacedWall;
	PlayerState m_state;

	std::vector<PlayerWall*> m_placedWalls;
	MapNode* m_mapNode;

	int m_playerIndex;

	///////////////FUNCTIONS/////////////
public:
	Player();
	Player(MapNode* p_startNode, float p_startAngle, int p_playerIndex);
	~Player();

	void Update(float p_dt,UserCMD p_userCMD); //Updates position and dropps walls
	void UpdatePosition(float p_dt, UserCMD p_userCMD);
	int ProperUpdatePosition(float p_dt, UserCMD p_userCMD);
	std::vector<BoundingOrientedBox*> GetWallsToCheck();
	void ChangeState(PlayerState p_state);


	//Gets yo
	XMMATRIX GetCamMatrix();
	XMFLOAT3 GetPos();
	XMFLOAT3 GetDirection();
	XMMATRIX GetWorldMatrix();		//game screen grabs and sends off to graphichandle
	MapNode* GetCurrentMapNode();
	BoundingOrientedBox* GetCollisionBox();
	PlayerWall* GetLastPlacedWall();
	float GetDistanceTraveled(); 
	float GetPlayerBoost();
	void SetPlayerBoost(float p_boost);
	void SetPlayerRacePosition(int p_pos);
	int GetRacePosition();
	float GetHudBoosterInfo();
	float GetHudWallInfo();
	bool GetImmortal();
	void Die();
	int GetPlayerIndex();

private:
	//rotates the up vector to the proper angle
	void FixUpVectorRotation(float p_angle);
	//translates the position along the new upvector
	void FixOffsetFromCenterSpline();
	void PlaceWall();
	void UpdateCollisionBox();
	void UpdateMapNode();
	void BumpedIntoPlayer(XMFLOAT3 p_force);
	void UpdateWorldMatrix();
	void UpdateWallMeter(float p_dt);

};

