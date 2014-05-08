#include "PhysicsManager.h"
#include "MathHelper.h"



PhysicsManager::PhysicsManager(void)
{
}


PhysicsManager::~PhysicsManager(void)
{
}



std::vector<DirectX::XMFLOAT3> PhysicsManager::GetForceInCollision(Player* p_p1, Player* p_p2)
{
	MathHelper t_mHelp = MathHelper();
	std::vector<DirectX::XMFLOAT3> r_return;

	XMFLOAT3 t_p1Pos = p_p1->GetPos();
	XMFLOAT3 t_p2Pos = p_p2->GetPos();
	XMFLOAT3 t_collisionVec;
	
	t_collisionVec = t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos)); //Vi ska projecera på denna vector NORMALISERAD!!!! Kanske inte ska in!

	float t_p1DirSkal = t_mHelp.DotProduct(p_p1->GetDirection(), t_collisionVec); //Första delen i projiceringsformeln (Skalären mellan den som ska projeceras och den som den ska projiceras på!)
	float t_p2DirSkal = t_mHelp.DotProduct(p_p2->GetDirection(), t_collisionVec); //Första delen i projiceringsformeln 


	float t_collisionSkal = t_mHelp.DotProduct(t_collisionVec, t_collisionVec); //AbsolutBelopp av vectorn som ska projiceras på
	t_collisionSkal *= t_collisionSkal; //abs upphöjt i två
	XMFLOAT3 t_p1Projection;
	XMFLOAT3 t_p2Projection;
	
	


	t_p1Projection = t_mHelp.FloatMultiVec((t_p1DirSkal / t_collisionSkal),t_collisionVec); //Projektionen från player 1 Skalären multiplicerad till vectorn som skulle projiceras på!
	t_p2Projection = t_mHelp.FloatMultiVec((t_p2DirSkal / t_collisionSkal),t_collisionVec); //Projektionen från player 2 Skalären multiplicerad till vectorn som skulle projiceras på!
	t_p2Projection.x = -t_p2Projection.x; t_p2Projection.y = -t_p2Projection.y;  t_p2Projection.z = -t_p2Projection.z; //vänder på ena vectorn

	float t_magnitude = t_mHelp.Abs(t_mHelp.VecSubVec(t_p1Projection, t_p2Projection));
	r_return.push_back(t_mHelp.FloatMultiVec(t_magnitude, t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos))));
	r_return.push_back(t_mHelp.FloatMultiVec(t_magnitude, t_mHelp.Normalize(t_mHelp.VecSubVec(t_p2Pos, t_p2Pos))));
	


	return r_return;
}
