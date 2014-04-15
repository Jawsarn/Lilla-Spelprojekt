#pragma once
#include <Windows.h>


#define WIN32_LEAN_AND_MEAN



#include <Xinput.h>

struct XboxStickDirection
	{
		float x;
		float y;

		XboxStickDirection() : x(0), y(0){}
		XboxStickDirection(float _x, float _y) : x(_x), y(_y){}

		XboxStickDirection& operator+(XboxStickDirection b)
		{
			x += b.x;
			y += b.y;
			return *this;
		}

		XboxStickDirection& operator-(XboxStickDirection b)
		{
			x -= b.x;
			y -= b.y;
			return *this;
		}

		XboxStickDirection& operator-=(XboxStickDirection b)
		{
			x -= b.x;
			y -= b.y;
			return *this;
		}

		XboxStickDirection& operator+=(XboxStickDirection b)
		{
			x += b.x;
			y += b.y;
			return *this;
		}

		XboxStickDirection& operator=(const XboxStickDirection& b)
		{
			x = b.x;
			y = b.y;
			return *this;
		}
	};


class Controller
{
public:
	//player number from 0-3 (0-1 used)
	//XBOXController();
	Controller(int p_controllerNumber);
	~Controller();

	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);

	//return values between -1 to 1
	XboxStickDirection CheckMovmentStickRight();

	//return values between -1 to 1
	XboxStickDirection CheckMovmentStickLeft();
	
private:
	XINPUT_STATE m_controllerState;
	int m_controllerNumber;

	XboxStickDirection CheckMovmentStick(float LX,float LY);
};

