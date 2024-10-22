#include "UserCMDHandler.h"
#include <Xinput.h>

UserCMDHandler::UserCMDHandler(void)
{
}


UserCMDHandler::~UserCMDHandler(void)
{
}

void UserCMDHandler::AlterUserCMD(UserCMD& o_userCMD)
{
	XINPUT_STATE state = o_userCMD.controller.GetState();
	DirectX::XMFLOAT2 leftJoystick = DirectX::XMFLOAT2(o_userCMD.controller.CheckMovmentStickLeft().x,o_userCMD.controller.CheckMovmentStickLeft().y);	
	DirectX::XMFLOAT2 rightJoystick = DirectX::XMFLOAT2(o_userCMD.controller.CheckMovmentStickRight().x,o_userCMD.controller.CheckMovmentStickRight().y);
	o_userCMD.Joystick = leftJoystick;
	o_userCMD.rightJoystick = rightJoystick;

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		o_userCMD.aButtonPressed = true;
	}
	else
	{
		o_userCMD.aButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		o_userCMD.bButtonPressed = true;
	}
	else
	{
		o_userCMD.bButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		o_userCMD.yButtonPressed = true;
	}
	else
	{
		o_userCMD.yButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		o_userCMD.xButtonPressed = true;
	}
	else
	{
		o_userCMD.xButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		o_userCMD.startButtonPressed= true;
	}
	else
	{
		o_userCMD.startButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		o_userCMD.backButtonPressed= true;
	}
	else
	{
		o_userCMD.backButtonPressed = false;
	}
	if (state.Gamepad.bLeftTrigger & VK_PAD_LTRIGGER)
	{
		//o_userCMD.controller.Vibrate(64000,0);
		o_userCMD.leftTriggerPressed = true;
	}
	else
	{
		o_userCMD.leftTriggerPressed = false;
	}
	if (state.Gamepad.bRightTrigger & VK_PAD_RTRIGGER)
	{
		o_userCMD.rightTriggerPressed = true;
	}
	else
	{
		o_userCMD.rightTriggerPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		o_userCMD.leftBumberPressed = true;
	}
	else
	{
		o_userCMD.leftBumberPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		o_userCMD.rightBumberPressed = true;
	}
	else
	{
		o_userCMD.rightBumberPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		o_userCMD.leftRetardButtonPressed = true;
	}
	else
	{
		o_userCMD.leftRetardButtonPressed = false;
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		o_userCMD.rightRetardButtonPressed = true;
	}
	else
	{
		o_userCMD.rightRetardButtonPressed = false;
	}

}
