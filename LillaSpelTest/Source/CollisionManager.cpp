#include "CollisionManager.h"


CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

int CollisionManager::PlayerVsObj(BoundingOrientedBox* p_player, std::vector<StaticObj*>* p_collisionBoxes)
{
	for (int i = 0; i < p_collisionBoxes->size(); i++)
	{
		if(p_player->Intersects(*p_collisionBoxes->at(i)->GetBox()))
		{
			return p_collisionBoxes->at(i)->GetType();
		}
		return 1;
	}
}

bool CollisionManager::PlayerVsPlayerWall(BoundingOrientedBox* p_player, std::vector<PlayerWall*>* p_collisionBoxes)
{
	for (int i = 0; i < p_collisionBoxes->size(); i++)
	{
		if(p_player->Intersects(*p_collisionBoxes->at(i)->GetBox()))
		{
			return true;
		}
	}
	return false;
}

std::vector<int> CollisionManager::PlayerVsPlayer(BoundingOrientedBox* p_p1, BoundingOrientedBox* p_p2,BoundingOrientedBox* p_p3,BoundingOrientedBox* p_p4)
{
	std::vector<int> r_return;
	if(p_p1->Intersects(*p_p2)==true)
	{
		r_return.push_back(1);
		r_return.push_back(2);
	}
	if(p_p1->Intersects(*p_p3)==true)
	{
		r_return.push_back(1);
		r_return.push_back(3);
	}
	if(p_p1->Intersects(*p_p4)==true) //detta är rätt fult ändra om nån känner för
	{
		r_return.push_back(1);
		r_return.push_back(4);
	}
	if(p_p2->Intersects(*p_p3)==true)
	{
		r_return.push_back(2);
		r_return.push_back(3);
	}
	if(p_p2->Intersects(*p_p4)==true)
	{
		r_return.push_back(2);
		r_return.push_back(4);
	}
	if(p_p3->Intersects(*p_p4)==true)
	{
		r_return.push_back(3);
		r_return.push_back(4);
	}

	return r_return;
}

bool CollisionManager::IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b)
{
	return true;
}