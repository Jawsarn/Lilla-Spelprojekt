#include "Controller.h"
#include <math.h>

Controller::Controller(int p_controllerNumber)
{
	m_controllerNumber = p_controllerNumber;
}


Controller::~Controller()
{

}

XINPUT_STATE Controller::GetState()
{
	ZeroMemory(&m_controllerState, sizeof(XINPUT_STATE));

	DWORD hr = XInputGetState(m_controllerNumber,&m_controllerState);

	return m_controllerState;
}


bool Controller::IsConnected()
{
	ZeroMemory(&m_controllerState, sizeof(XINPUT_STATE));

	DWORD hr = XInputGetState(m_controllerNumber,&m_controllerState);

	if (hr == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Controller::Vibrate(int leftVal, int rightVal)
{
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = leftVal;
    vibration.wRightMotorSpeed = rightVal;
	
	XInputSetState(m_controllerNumber, &vibration);
}

XboxStickDirection Controller::CheckMovmentStickLeft()
{
	float LX = m_controllerState.Gamepad.sThumbLX;
	float LY = m_controllerState.Gamepad.sThumbLY;

	return CheckMovmentStick(LX, LY);
}

XboxStickDirection Controller::CheckMovmentStickRight()
{
	float LX = m_controllerState.Gamepad.sThumbRX;
	float LY = m_controllerState.Gamepad.sThumbRY;

	return CheckMovmentStick(LX, LY);
}

XboxStickDirection Controller::CheckMovmentStick(float LX,float LY)
{
	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	//float normalizedMagnitude = 0;

	

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	{
	  //clip the magnitude at its expected maximum value
	  /*if (magnitude > 32767) 
	  {
		  magnitude = 32767;
	  }*/
  
	  //adjust magnitude relative to the end of the dead zone
	  //magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  //normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		return XboxStickDirection(normalizedLX,normalizedLY);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		/*magnitude = 0.0;
		normalizedMagnitude = 0.0;*/
		return XboxStickDirection(0,0);
	}
	//repeat for right thumb stick
}