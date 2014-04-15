#pragma once
#include "UserCMD.h"
#include <vector>
class Screen
{
protected:
	
public:
	Screen(void);
	~Screen(void);

	int virtual Update(float p_dt, std::vector<UserCMD> p_userCMDS);
	void virtual Draw();
};

