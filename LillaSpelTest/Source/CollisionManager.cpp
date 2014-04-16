#include "CollisionManager.h"


CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

int CollisionManager::PlayerVsObj(BoundingOrientedBox* p_player, std::vector<BoundingOrientedBox*> p_collisionBoxes)
{
	return 1;
}

std::vector<int> CollisionManager::PlayerVsPlayer(BoundingOrientedBox* p_p1, BoundingOrientedBox* p_p2,BoundingOrientedBox* p_p3,BoundingOrientedBox* p_p4)
{
	std::vector<int> r_return;
	return r_return;
}

bool CollisionManager::IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b)
{
	return true;
}