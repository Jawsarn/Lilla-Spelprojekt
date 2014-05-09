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
	
	t_collisionVec = t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos)); //Vi ska projecera p� denna vector NORMALISERAD!!!! Kanske inte ska in!

	float t_p1DirSkal = t_mHelp.DotProduct(p_p1->GetDirection(), t_collisionVec); //F�rsta delen i projiceringsformeln (Skal�ren mellan den som ska projeceras och den som den ska projiceras p�!)
	float t_p2DirSkal = t_mHelp.DotProduct(p_p2->GetDirection(), t_collisionVec); //F�rsta delen i projiceringsformeln 


	float t_collisionSkal = t_mHelp.DotProduct(t_collisionVec, t_collisionVec); //AbsolutBelopp av vectorn som ska projiceras p�
	t_collisionSkal *= t_collisionSkal; //abs upph�jt i tv�
	XMFLOAT3 t_p1Projection;
	XMFLOAT3 t_p2Projection;
	
	


	t_p1Projection = t_mHelp.FloatMultiVec((t_p1DirSkal / t_collisionSkal),t_collisionVec); //Projektionen fr�n player 1 Skal�ren multiplicerad till vectorn som skulle projiceras p�!
	t_p2Projection = t_mHelp.FloatMultiVec((t_p2DirSkal / t_collisionSkal),t_collisionVec); //Projektionen fr�n player 2 Skal�ren multiplicerad till vectorn som skulle projiceras p�!
	t_p2Projection.x = -t_p2Projection.x; t_p2Projection.y = -t_p2Projection.y;  t_p2Projection.z = -t_p2Projection.z; //v�nder p� ena vectorn

	float t_magnitude = t_mHelp.Abs(t_mHelp.VecSubVec(t_p1Projection, t_p2Projection));
	XMFLOAT3 t_p1Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude, t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos))));
	XMFLOAT3 t_p2Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude, t_mHelp.Normalize(t_mHelp.VecSubVec(t_p2Pos, t_p2Pos))));
	

	///////H�r b�rjar projicering p� playersens dirvectorer!
	XMFLOAT3 t_p1Direction = p_p1->GetDirection();
	XMFLOAT3 t_p2Direction = p_p2->GetDirection();

	t_p1DirSkal = t_mHelp.DotProduct(t_p1Magnitude,t_p1Direction);
	t_p2DirSkal = t_mHelp.DotProduct(t_p2Magnitude,t_p2Direction);
	//F�rsta projeceringen... F�r player 1
	float t_p1ProjectionDivider = t_mHelp.DotProduct(t_p1Direction, t_p1Direction); //abs det som ska vara under br�kstr�cket i proj formeln
	t_p1ProjectionDivider *= t_p1ProjectionDivider; // ABs ^2

	XMFLOAT3 t_p1DirectionProjection = t_mHelp.FloatMultiVec((t_p1DirSkal / t_p1ProjectionDivider), t_p1Direction); //Klar projecering av kraftvektor p� p1s direction.. Den h�r ska �vers�ttas till m_speed
	///////
	//p_p1->SetSpeed( t_mHelp.DotProduct(t_p1DirectionProjection, t_p1DirectionProjection);
	//F�rsta projeceringen.. F�rplayer 2
	float t_p2ProjectionDivider = t_mHelp.DotProduct(t_p2Direction, t_p2Direction); //abs det som ska vara under br�kstr�cket i proj formeln
	t_p2ProjectionDivider *= t_p2ProjectionDivider; // ABs ^2
	XMFLOAT3 t_p2DirectionProjection = t_mHelp.FloatMultiVec((t_p1DirSkal / t_p2ProjectionDivider), t_p2Direction); //Klar Projecering av kraftvektor p� p2s direction samma som f�r p1 ^^^^ d�r uppe
	//////////////////////////////////////////////////////////////////////////
	//p_p2->SetSpeed( t_mHelp.DotProduct(t_p2DirectionProjection, t_p2DirectionProjection);

	//////Projecering p� Radiusvectorn!!!!! anv�nds f�r angle p�plusning
	//XMFLOAT3 t_p1Radius = p_p1->GetRadius();
	//XMFLOAT3 t_p2Radius = p_p2->GetRadius();





	return r_return;
}
