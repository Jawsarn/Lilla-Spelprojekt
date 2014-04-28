#pragma once
#include "Player.h"
#include "PlayerWall.h"


class PhysicsManager
{
public:
	PhysicsManager(void);
	~PhysicsManager(void);
	std::vector<DirectX::XMFLOAT3> GetForceInCollision(Player* p_p1, Player* p_p2);
};

