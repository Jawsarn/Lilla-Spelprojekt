#pragma once
#include "object.h"
#include "PlayerWall.h"
#include "MapNode.h"
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

	float m_angle;
	float m_boostMeter;
	float m_wallMeter;
	float m_coolDown;
	float m_speed;
	///Timers///
	float m_deathTimer;
	float m_immortalTimer;
	
	DirectX::XMFLOAT3 m_acceleration;
	DirectX::XMFLOAT3 m_color;
	DirectX::XMFLOAT3 m_upVector;

	PlayerState m_state;

	std::vector<PlayerWall*> m_placedWalls;
	MapNode* m_mapNode;

	///////////////FUNCTIONS/////////////
public:
	Player();
	Player(MapNode* p_startNode, float p_startAngle);
	~Player();

	void Update(float p_dt,UserCMD p_userCMD); //Updates position and dropps walls
	void UpdatePosition(float p_dt, UserCMD p_userCMD);
	std::vector<BoundingOrientedBox*> GetWallsToCheck();
	void ChangeState(PlayerState p_state);
	

	//Gets yo
	XMFLOAT3 GetPos();
	XMFLOAT3 GetDirection();

private:
	void PlaceWall();
	void UpdateCollisionBox();
	void CheckMapNode();
};

