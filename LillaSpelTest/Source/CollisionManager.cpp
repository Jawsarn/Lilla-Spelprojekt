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
	for (int i = 0; i <	p_playerList.size(); i++)
	{
		for (int j = 0; j < p_playerList.size(); j++)
		{
			if(i!=j)
				if(p_playerList[i]->GetCollisionBox()->Intersects(*p_playerList[j]->GetCollisionBox()))
				{
					SetPlayerVsPlayer(p_playerList[i], p_playerList[j]);
				}
		}
	}
}

bool CollisionManager::IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b)
{
	return true;
}

void CollisionManager::SetPlayerVsPlayer(Player* p_currentPlayer, Player* p_intersectingPlayer)
{
	//current player is the one to get sent off. intersecting players is current player in another method call. epic explonation yo
	float t_currentPlayerAngle = fmod(p_currentPlayer->GetAngle(), 2*3.1415);
	float t_intersectingPlayerAngle = fmod(p_intersectingPlayer->GetAngle(), 2*3.1415);

	float t_direction = t_currentPlayerAngle-t_intersectingPlayerAngle;
	//get direction -1 or 1
	t_direction /= abs(t_direction);
	//p_intersectingPlayer->get

	float t_testBumpAngle = 0.1;

	p_currentPlayer->StartCollisionAftermath(t_testBumpAngle*t_direction);
	p_currentPlayer->AngleMoveBack();



}
void CollisionManager::ShockWaveCollision(std::vector<Player*> p_playerList, int p_playerWithShockwave)
{
	BoundingSphere t_shockWaveSphere = BoundingSphere(p_playerList[p_playerWithShockwave]->GetPos(), 1.5);
	for (int i = 0; i < p_playerList.size(); i++)
	{
		if(i!=p_playerWithShockwave)
		{
			if(p_playerList[i]->GetCollisionBox()->Intersects(t_shockWaveSphere))
			{
				m_physMan->SetPlayerCollisions(p_playerList[p_playerWithShockwave],p_playerList[i], 0, 2);
			}
		}
	}
}