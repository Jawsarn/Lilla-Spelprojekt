#pragma once
#include <vector>
#include "UserCMD.h"
#include "Controller.h"

class UserCMDHandler
{
public:
	UserCMDHandler(void);
	~UserCMDHandler(void);
	void GetUserCMDs(std::vector<UserCMD> o_userCMDs, std::vector<Controller*> controllers); //Takes x numbers of controllers and returns the userCMDs for each one
};

