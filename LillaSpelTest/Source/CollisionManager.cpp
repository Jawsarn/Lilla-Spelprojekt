#include "CollisionManager.h"


CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

int CollisionManager::PlayerVsObj(BoundingOrientedBox* p_player, std::vector<StaticObj*> p_collisionBoxes)
{
	for (int i = 0; i < p_collisionBoxes.size(); i++)
	{
		if(p_player->Intersects(*p_collisionBoxes[i]->GetBox())==true) ///Kolla om det stämmer med pointern
		{
			return p_collisionBoxes[i]->GetType();    ///har john gjort getsen?
		}
	}
	return 1;
}

bool CollisionManager::PlayerVsPlayerWall(BoundingOrientedBox* p_player, std::vector<PlayerWall*> p_collisionBoxes)
{
	for (int i = 0; i < p_collisionBoxes.size(); i++)
	{
		if(p_player->Intersects(*p_collisionBoxes[i]->GetBox())==true)
		{
			return true;
		}
	}
	return false;
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