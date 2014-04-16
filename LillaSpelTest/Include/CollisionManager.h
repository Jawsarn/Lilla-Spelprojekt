#pragma once
#include <vector>
#include <DirectXCollision.h>
#include <DirectXMath.h>

using namespace DirectX;
class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);



	int PlayerVsObj(BoundingOrientedBox* p_player, std::vector<BoundingOrientedBox*> p_collisionBoxes);
	std::vector<int> PlayerVsPlayer(BoundingOrientedBox* p_p1, BoundingOrientedBox* p_p2,BoundingOrientedBox* p_p3,BoundingOrientedBox* p_p4);

private:
	bool IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b);
};

