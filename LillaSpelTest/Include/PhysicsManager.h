#pragma once
#include "Player.h"
#include "PlayerWall.h"


class PhysicsManager
{
public:
	PhysicsManager(void);
	~PhysicsManager(void);
	void SetPlayerCollisions(Player* p_p1, Player* p_p2);
};

