#pragma once

#include <DirectXMath.h>
#include "Controller.h"

struct UserCMD
{
	bool leftTriggerPressed;	
	bool rightTriggerPressed;	
	bool leftBumberPressed;
	bool rightBumberPressed;
	bool aButtonPressed;	
	bool bButtonPressed;	
	bool xButtonPressed ;
	bool yButtonPressed;
	bool startButtonPressed;
	bool backButtonPressed;
	bool leftRetardButtonPressed;
	bool rightRetardButtonPressed;
	DirectX::XMFLOAT2 Joystick;
	DirectX::XMFLOAT2 rightJoystick;

	Controller controller;
	UserCMD()
	{

	}
	UserCMD(int p_controllerNumber)
	{
		leftTriggerPressed = false;	
		rightTriggerPressed = false;	
		leftBumberPressed = false;
		rightBumberPressed = false;
		aButtonPressed = false;	
		bButtonPressed = false;	
		xButtonPressed = false;
		yButtonPressed = false;
		startButtonPressed = false;
		backButtonPressed = false;
		leftRetardButtonPressed = false;
		rightRetardButtonPressed = false;
		Joystick = DirectX::XMFLOAT2(0,0);
		rightJoystick = DirectX::XMFLOAT2(0,0);
		controller = Controller(p_controllerNumber);
	}
};
