#pragma once
#include "object.h"
#include "PlayerWall.h"
#include "MapNode.h"
#include "PlayerWall.h"
#include "UserCMD.h"

enum PlayerState{Normal,Dead,Immortal};

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
	Player(void);
	Player(MapNode* p_startNode, float p_startAngle);
	~Player(void);

	void Update(float p_dt,UserCMD p_userCMD); //Updates position and dropps walls
	std::vector<Box*> GetWallsToCheck();
	void ChangeState(PlayerState p_state);

private:
	void PlaceWall();
	void UpdateCollisionBox();
};

