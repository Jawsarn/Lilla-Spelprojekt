#pragma once
#include <vector>
#include "UserCMD.h"

class MysteriskTest
{
public:
	MysteriskTest(void);
	~MysteriskTest(void);
	void Run(std::vector<UserCMD>* players, float dt);
};

