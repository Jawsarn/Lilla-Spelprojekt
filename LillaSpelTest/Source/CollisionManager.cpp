#include "CollisionManager.h"


CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

int CollisionManager::PlayerVsObj(Box* p_player, std::vector<Box*> p_collisionBoxes)
{
	return 1;
}

std::vector<int> CollisionManager::PlayerVsPlayer(Box* p_p1, Box* p_p2,Box* p_p3,Box* p_p4)
{
	std::vector<int> r_return;
	return r_return;
}

bool CollisionManager::IntersectionTest(Box* a, Box* b)
{
	return true;
}