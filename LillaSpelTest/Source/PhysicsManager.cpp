#include "PhysicsManager.h"
#include "XMfloat3MathHelper.h"



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
	XMFLOAT3 t_collisionVec;

	XMFLOAT3 t_p1Direction = p_p1.GetDirection();
	XMFLOAT3 t_p2Direction = p_p1.GetDirection();

	XMFLOAT3 t_p1Projection;
	XMFLOAT3 t_p2Projection;
	
	t_collisionVec = XMFLOAT3(t_p1Pos.x-t_p2Pos.x, t_p1Pos.y - t_p2Pos.y, t_p1Pos.z - t_p2Pos.z); //Vi ska projecera på denna vector
	
	//Skalär av vectorn som ska projeceras på collisionvec
	t_p1Projection = ((




	return r_return;
}
