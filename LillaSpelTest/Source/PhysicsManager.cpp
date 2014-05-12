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

	//hämtar positioner från krockoffren
	XMFLOAT3 t_p1Pos = p_p1->GetPos();
	XMFLOAT3 t_p2Pos = p_p2->GetPos();
	XMFLOAT3 t_collisionVec;
	//skapar en projiceringsvector som är vectorn mellan de 2 krockoffren KANSKE INTE SKA VARA NORMALISERAD
	t_collisionVec = t_mHelp.Normalize(t_mHelp.VecSubVec(t_p1Pos, t_p2Pos));  

	//gör Projiceringen 
	XMFLOAT3 t_p1Projection = t_mHelp.Projection(p_p1->GetDirection(),t_collisionVec);
	XMFLOAT3 t_p2Projection = t_mHelp.Projection(p_p2->GetDirection(),t_collisionVec);
	//vänder på ena vectorn, ena offret ska åka tvärt om mot vad den andra gör
	t_p2Projection.x = -t_p2Projection.x; t_p2Projection.y = -t_p2Projection.y;  t_p2Projection.z = -t_p2Projection.z; //kanskeinte ska göras

	//skapar ett värde över hur "stor" kollisionen är
	float t_magnitude = t_mHelp.Abs(t_mHelp.VecSubVec(t_p1Projection, t_p2Projection));

	//Skapar 2 vectorer från projiceringsvectorerna gånger hur stark krocken var
	XMFLOAT3 t_p1Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude* p_p1MagnitudeMultiplier, t_mHelp.VecSubVec(t_p1Pos, t_p2Pos))); //kanske add om jag tar bort att jag vänder ^
	XMFLOAT3 t_p2Magnitude = XMFLOAT3 (t_mHelp.FloatMultiVec(t_magnitude* p_p2MagnitudeMultiplier, t_mHelp.VecSubVec(t_p2Pos, t_p1Pos)));
	

	//Här startar uppdelningen av magnitudevektorerna till angle och speed (playerradius & Playerdirection )
	///////Här börjar projicering på playersens dirvectorer!
	XMFLOAT3 t_p1Direction = p_p1->GetUnmodifiedTargetVector();
	XMFLOAT3 t_p2Direction = p_p2->GetUnmodifiedTargetVector();
	//Klar projecering av kraftvektor på p1s direction.. Den här ska översättas till m_speed
	XMFLOAT3 t_p1DirectionProjection = t_mHelp.Projection(t_p1Direction, t_p1Magnitude); 
	//HÄR ÄR TWEEEEEEEKZZZZ
	p_p1->SetSpeed( t_mHelp.DotProduct(t_p1DirectionProjection, t_p1DirectionProjection));
	//Klar Projecering av kraftvektor på p2s direction samma som för p1 ^^^^ där uppe
	XMFLOAT3 t_p2DirectionProjection = t_mHelp.Projection(t_p2Direction,t_p2Magnitude); 
	//TWEEEEEKZZZZZZ
	p_p2->SetSpeed( t_mHelp.DotProduct(t_p2DirectionProjection, t_p2DirectionProjection));

	//////Projecering på Radiusvectorn!!!!! används för angle påplusning
	XMFLOAT3 t_p1Radius = p_p1->GetRadiusVector();
	XMFLOAT3 t_p2Radius = p_p2->GetRadiusVector();
	//Klar Projecering av Kraftvektorn på p1s Radius, ska översättas till m_angle
	XMFLOAT3 t_p1RadiusProjection = t_mHelp.Projection(t_p1Magnitude, t_p1Radius); 
	XMFLOAT3 t_p2RadiusProjection = t_mHelp.Projection(t_p2Magnitude, t_p2Radius);

	float t_p1Angle = t_mHelp.Abs(t_p1RadiusProjection);
	float t_p2Angle = t_mHelp.Abs(t_p2RadiusProjection);
	p_p1->StartCollisionAftermath(t_p1Angle/10);
	p_p2->StartCollisionAftermath(t_p2Angle/10);
}
