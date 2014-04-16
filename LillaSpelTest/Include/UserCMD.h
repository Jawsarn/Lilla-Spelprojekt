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
		controller = Controller(p_controllerNumber);
	}
};
