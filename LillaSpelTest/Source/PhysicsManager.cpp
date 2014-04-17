#include "PhysicsManager.h"
#include "MathHelper.h"



PhysicsManager::PhysicsManager(void)
{
}


PhysicsManager::~PhysicsManager(void)
{
}



std::vector<DirectX::XMFLOAT3> PhysicsManager::GetForceInCollision(Player p_p1, Player p_p2)
{
	MathHelper t_mHelp = MathHelper();
	std::vector<DirectX::XMFLOAT3> r_return;

	XMFLOAT3 t_p1Pos = p_p1.GetPos();
	XMFLOAT3 t_collisionVec;

	float t_p1DirSkal = t_mHelp.SkalarProdukt(p_p1.GetDirection(), t_collisionVec);
	float t_collisionSkal = t_mHelp.SkalarProdukt(t_collisionVec, t_collisionVec);

	XMFLOAT3 t_p1Projection;
	XMFLOAT3 t_p2Projection;
	
	t_collisionVec = XMFLOAT3(t_p1Pos.x-t_p2Pos.x, t_p1Pos.y - t_p2Pos.y, t_p1Pos.z - t_p2Pos.z); //Vi ska projecera på denna vector
	
	//Skalär av vectorn som ska projeceras på collisionvec
	t_p1Projection = (t_p1DirSkal / t_collisionSkal)*t_collisionVec;




	return r_return;
}
