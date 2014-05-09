#include "CollisionManager.h"


CollisionManager::CollisionManager(void)
{
	m_physMan = new PhysicsManager();
}


CollisionManager::~CollisionManager(void)
{
}

int CollisionManager::PlayerVsObj(BoundingOrientedBox* p_player, std::vector<StaticObj*>* p_collisionBoxes)
{
	for (int i = 0; i < p_collisionBoxes->size(); i++)
	{
		if(p_collisionBoxes->at(i)->GetType() == 1) //uber hax ugly code to make sure we aren't affected by  hole boxes
		{
			if(p_player->Intersects(*p_collisionBoxes->at(i)->GetBox()))
			{
				return p_collisionBoxes->at(i)->GetType();
			}
		}
	}
	return 1;
}

int CollisionManager::PlayerVsPlayerWall(BoundingOrientedBox* p_player, std::vector<PlayerWall*> p_collisionBoxes, int p_playerIndex) //copies the list. Might be bad, but its a list of pointers. No biggie
{
	int t_wallSpheresHit = 0;
	for (int i = 0; i < p_collisionBoxes.size(); i++)
	{
		if(p_playerIndex != p_collisionBoxes.at(i)->GetPlayerIndex())
		{
			if(p_player->Intersects(*p_collisionBoxes.at(i)->GetSphere()))
			{
				t_wallSpheresHit++;
				if(p_player->Intersects(*p_collisionBoxes.at(i)->GetBox()))
					return -1;
			}
		}
	}
	return t_wallSpheresHit;
}

void CollisionManager::PlayerVsPlayer(std::vector<Player*> p_playerList)
{
	for (int i = 0; i <	p_playerList.size()-1; i++)
	{
		for (int j = i+1; j < p_playerList.size(); j++)
		{
			if(p_playerList[i]->GetCollisionBox()->Intersects(*p_playerList[j]->GetCollisionBox()))
			{
				m_physMan->SetPlayerCollisions(p_playerList[i],p_playerList[j]);
			}
		}
	}









	//std::vector<int> r_return;
	//if(p_p1->GetCollisionBox()->Intersects(*p_p2->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p1, p_p2);	
	//}
	//if(p_p1->GetCollisionBox()->Intersects(*p_p3->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p1, p_p3);	
	//}
	//if(p_p1->GetCollisionBox()->Intersects(*p_p4->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p1, p_p4);	
	//}
	//if(p_p2->GetCollisionBox()->Intersects(*p_p3->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p2, p_p3);	
	//}
	//if(p_p2->GetCollisionBox()->Intersects(*p_p4->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p2, p_p4);	
	//}
	//if(p_p3->GetCollisionBox()->Intersects(*p_p4->GetCollisionBox())==true)
	//{
	//	m_physMan->SetPlayerCollisions(p_p3, p_p4);	
	//}
}

bool CollisionManager::IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b)
{
	return true;
}