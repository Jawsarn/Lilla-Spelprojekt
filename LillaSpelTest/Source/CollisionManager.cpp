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
	if(p_collisionBoxes->size()>0)
	{


		for (int i = 0; i < p_collisionBoxes->size(); i++)
		{
			//if(p_collisionBoxes->at(i)->GetType() == 1)
			if(true) //uber hax ugly code to make sure we aren't affected by  hole boxes
			{
				if(p_player->Intersects(*p_collisionBoxes->at(i)->GetBox()))
				{
					return p_collisionBoxes->at(i)->GetType();
				}
			}
		}
	}
	return -1;
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
	XMMATRIX t_currentPlayerInverseMatrix = XMMatrixInverse(nullptr, p_currentPlayer->GetWorldMatrix());

	XMVECTOR t_currentPlayerLocalPosition = XMVector3Transform(XMLoadFloat3(&p_currentPlayer->GetPos()),t_currentPlayerInverseMatrix);
	XMVECTOR t_intersectingPlayerLocalPosition = XMVector3Transform(XMLoadFloat3(&p_intersectingPlayer->GetPos()),t_currentPlayerInverseMatrix);

	XMFLOAT3 t_testCurrPlayPos;
	XMFLOAT3 t_testInterPlayPos;

	XMStoreFloat3(&t_testCurrPlayPos, t_currentPlayerLocalPosition);
	XMStoreFloat3(&t_testInterPlayPos, t_intersectingPlayerLocalPosition);


	float t_targetDirection = (t_testInterPlayPos.z/abs(t_testInterPlayPos.z));
	float t_targetForce = abs(p_intersectingPlayer->GetSpeed());

	float t_sideDirection = -1*(t_testInterPlayPos.x/abs(t_testInterPlayPos.x));
	float t_sideForce = abs(p_intersectingPlayer->GetDeltaAngle());

	p_currentPlayer->StartCollisionAftermath(t_sideForce,t_targetForce ,t_sideDirection, t_targetDirection);
	p_currentPlayer->AngleMoveBack();
	//possibly needs a position move back. Don't really see how that happens though...

}
void CollisionManager::ShockWaveCollision(std::vector<Player*> p_playerList, int p_playerWithShockwave)
{
	BoundingSphere t_shockWaveSphere = BoundingSphere(p_playerList[p_playerWithShockwave]->GetPos(), 3.5);
	for (int i = 0; i < p_playerList.size(); i++)
	{
		if(i!=p_playerWithShockwave)
		{
			if(p_playerList[i]->GetCollisionBox()->Intersects(t_shockWaveSphere))
			{
				SetShockWaveCollision(p_playerList[p_playerWithShockwave],p_playerList[i]);
			}
		}
	}
}

void CollisionManager::SetShockWaveCollision(Player* p_playerWithShockWave, Player* p_intersectingPlayer)
{
	XMMATRIX t_p_playerWithShockWaveInverseMatrix = XMMatrixInverse(nullptr, p_playerWithShockWave->GetWorldMatrix());
	XMVECTOR t_p_playerWithShockWaveLocalPosition = XMVector3Transform(XMLoadFloat3(&p_playerWithShockWave->GetPos()),t_p_playerWithShockWaveInverseMatrix);
	XMVECTOR t_intersectingPlayerLocalPosition = XMVector3Transform(XMLoadFloat3(&p_intersectingPlayer->GetPos()),t_p_playerWithShockWaveInverseMatrix);

	XMFLOAT3 t_ShockWavePlayPos;
	XMFLOAT3 t_InterPlayPos;

	XMStoreFloat3(&t_ShockWavePlayPos, t_p_playerWithShockWaveLocalPosition);
	XMStoreFloat3(&t_InterPlayPos, t_intersectingPlayerLocalPosition);


	float t_targetDirection = (t_InterPlayPos.z/abs(t_InterPlayPos.z));

	float t_sideDirection = (t_InterPlayPos.x/abs(t_InterPlayPos.x));

	p_intersectingPlayer->StartShockWaveAftermath(t_sideDirection, t_targetDirection, abs(t_InterPlayPos.z), abs(t_InterPlayPos.x));
	p_playerWithShockWave->SetShockwaveCooldown();
}