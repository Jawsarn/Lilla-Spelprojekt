#pragma once

#include <DirectXMath.h>
#include "Controller.h"

struct UserCMD
{
	bool leftTriggerPressed;	
	bool rightTriggerPressed;	
	bool aButtonPressed;	
	bool bButtonPressed;	
	bool xButtonPressed ;
	bool yButtonPressed;
	bool startButtonPressed;
	bool backButtonPressed;
	DirectX::XMFLOAT2 Joystick;

	Controller controller;

	UserCMD(int p_controllerNumber)
	{
		leftTriggerPressed = false;	
		rightTriggerPressed = false;	
		aButtonPressed = false;	
		bButtonPressed = false;	
		xButtonPressed = false;
		yButtonPressed = false;
		startButtonPressed = false;
		backButtonPressed = false;
		Joystick = DirectX::XMFLOAT2(0,0);
		controller = Controller(p_controllerNumber);
	}
};
