#pragma once
#include "Box.h"
#include <vector>

class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);

	int PlayerVsObj(Box* p_player, std::vector<Box*> p_collisionBoxes);
	std::vector<int> PlayerVsPlayer(Box* p_p1, Box* p_p2,Box* p_p3,Box* p_p4);

private:
	bool IntersectionTest(Box* a, Box* b);
};

