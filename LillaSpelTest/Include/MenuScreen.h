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
	int m_changedButtonSound;
public:
	MenuScreen(void);
	MenuScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager);
	~MenuScreen(void);
	int virtual Update( float p_dt, std::vector<UserCMD>* p_userCMDs);
	void virtual Draw();
	void virtual Initialize() = 0;
protected:
	std::string virtual NavigateMenu(UserCMD& p_userCMD,unsigned int p_hudHandle);
	void AddButton(std::string p_buttonName, DirectX::XMFLOAT2 p_centerPoint, float offsetX, float offsetY, const wchar_t* texture1, const wchar_t* texture2);
	void MakeHud(unsigned int & o_hudID);
	void FixButtonPointers();
private:
	bool FloatBetweenValues(float floatToCheck, float min, float max); 
	bool CheckIfButtonIsClosest(DirectX::XMFLOAT2 p_currentButtonsCenter,DirectX::XMFLOAT2 p_buttonToCheckCenter,Button* p_previousConnectedButton);
};

