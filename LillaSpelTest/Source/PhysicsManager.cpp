#include "PhysicsManager.h"
#include "MathHelper.h"



PhysicsManager::PhysicsManager(void)
{
}


PhysicsManager::~PhysicsManager(void)
{
}



void PhysicsManager::SetPlayerCollisions(Player* p_p1, Player* p_p2, float p_p1MagnitudeMultiplier, float p_p2MagnitudeMultiplier)
{
	MathHelper t_mHelp = MathHelper();

	//h�mtar positioner fr�n krockoffren
	XMFLOAT3 t_p1Pos = p_p1->GetPos();
	XMFLOAT3 t_p2Pos = p_p2->GetPos();
	XMFLOAT3 t_collisionVec;
	//skapar en projiceringsvector som �r vectorn mellan de 2 krockoffren KANSKE INTE SKA VARA NORMALISERAD
	t_collisionVec = t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos));  

	//g�r Projiceringen 
	XMFLOAT3 t_p1Projection = t_mHelp.Projection(p_p1->GetDirection(),t_collisionVec);
	XMFLOAT3 t_p2Projection = t_mHelp.Projection(p_p2->GetDirection(),t_collisionVec);
	//v�nder p� ena vectorn, ena offret ska �ka tv�rt om mot vad den andra g�r
	t_p2Projection.x = -t_p2Projection.x; t_p2Projection.y = -t_p2Projection.y;  t_p2Projection.z = -t_p2Projection.z; //kanskeinte ska g�ras

	//skapar ett v�rde �ver hur "stor" kollisionen �r
	float t_magnitude = t_mHelp.Abs(t_mHelp.VecSubVec(t_p1Projection, t_p2Projection));

	//Skapar 2 vectorer fr�n projiceringsvectorerna g�nger hur stark krocken var
	XMFLOAT3 t_p1Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude* p_p1MagnitudeMultiplier, t_mHelp.VecSubVec(t_p1Pos, t_p2Pos))); //kanske add om jag tar bort att jag v�nder ^
	XMFLOAT3 t_p2Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude* p_p2MagnitudeMultiplier, t_mHelp.VecSubVec(t_p2Pos, t_p1Pos)));
	

	//H�r startar uppdelningen av magnitudevektorerna till angle och speed (playerradius & Playerdirection )
	///////H�r b�rjar projicering p� playersens dirvectorer!
	XMFLOAT3 t_p1Direction = p_p1->GetUnmodifiedTargetVector();
	XMFLOAT3 t_p2Direction = p_p2->GetUnmodifiedTargetVector();
	//Klar projecering av kraftvektor p� p1s direction.. Den h�r ska �vers�ttas till m_speed
	XMFLOAT3 t_p1DirectionProjection = t_mHelp.Projection(t_p1Direction, t_p1Magnitude); 
	//H�R �R TWEEEEEEEKZZZZ
	p_p1->SetSpeed( t_mHelp.DotProduct(t_p1DirectionProjection, t_p1DirectionProjection));
	//Klar Projecering av kraftvektor p� p2s direction samma som f�r p1 ^^^^ d�r uppe
	XMFLOAT3 t_p2DirectionProjection = t_mHelp.Projection(t_p2Direction,t_p2Magnitude); 
	//TWEEEEEKZZZZZZ
	p_p2->SetSpeed( t_mHelp.DotProduct(t_p2DirectionProjection, t_p2DirectionProjection));

	//////Projecering p� Radiusvectorn!!!!! anv�nds f�r angle p�plusning
	XMFLOAT3 t_p1Radius = p_p1->GetRadiusVector();
	XMFLOAT3 t_p2Radius = p_p2->GetRadiusVector();
	//Klar Projecering av Kraftvektorn p� p1s Radius, ska �vers�ttas till m_angle
	XMFLOAT3 t_p1RadiusProjection = t_mHelp.Projection(t_p1Magnitude, t_p1Radius); 
	XMFLOAT3 t_p2RadiusProjection = t_mHelp.Projection(t_p2Magnitude, t_p2Radius);

	float t_p1Angle = t_mHelp.Abs(t_p1RadiusProjection);
	float t_p2Angle = t_mHelp.Abs(t_p2RadiusProjection);
	p_p1->StartCollisionAftermath(t_p1Angle/10);
	p_p2->StartCollisionAftermath(t_p2Angle/10);
}
