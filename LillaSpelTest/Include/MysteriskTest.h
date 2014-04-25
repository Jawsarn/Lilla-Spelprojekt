#pragma once
#include <vector>
#include "UserCMD.h"
#include "GraphicHandle.h"

class MysteriskTest
{
public:
	MysteriskTest(void);
	MysteriskTest(GraphicHandle* grphandle);
	~MysteriskTest(void);
	void Run(std::vector<UserCMD>* players, float dt);
};

