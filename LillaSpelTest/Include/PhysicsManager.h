#pragma once
#include "Player.h"
#include "PlayerWall.h"


class PhysicsManager
{
public:
	PhysicsManager(void);
	~PhysicsManager(void);
	void SetPlayerCollisions(Player* p_p1, Player* p_p2,float p_p1MagnitudeMultiplier, float p_p2MagnitudeMultiplier);
	void SetPlayerCollisions(Player* p_p1, Player* p_p2);
};

