#pragma once
#include "screen.h"
#include "Struct_Button.h"
#include "Struct_GameInfo.h"
#include <vector>
#include "UserCMD.h"
#include <math.h>


class MenuScreen :
	public Screen
{

protected:
	Button* currentButton;
	std::vector<Button*> buttonList;
	GameInfo* m_gameInfo;
	float timeSinceLastChange[4];
public:
	MenuScreen(void);
	MenuScreen(GameInfo* p_gameInfo);
	~MenuScreen(void);
	int virtual Update(std::vector<UserCMD>* p_userCMDs, float p_dt);
	void virtual Draw();
protected:
	std::string virtual NavigateMenu(UserCMD& p_userCMD);
	void AddButton(std::string p_buttonName, DirectX::XMFLOAT2 p_centerPoint, float offsetX, float offsetY);
	void FixButtonPointers();
private:
	bool FloatBetweenValues(float floatToCheck, float min, float max); 
};

