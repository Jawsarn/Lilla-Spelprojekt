#include "PhysicsManager.h"


PhysicsManager::PhysicsManager(void)
{
}


PhysicsManager::~PhysicsManager(void)
{
}

std::vector<DirectX::XMFLOAT3> PhysicsManager::GetForceInCollision(Player p_p1, Player p_p2)
{
	std::vector<DirectX::XMFLOAT3> r_return;
	XMFLOAT3 t_p1Pos = p_p1.GetPos();
	XMFLOAT3 t_p2Pos = p_p2.GetPos();
	return r_return;
}
