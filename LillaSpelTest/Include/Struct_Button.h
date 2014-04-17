#pragma once
#include <string>
#include <DirectXMath.h>

struct Button
{
	
	std::string buttonName;
	Button* upButton;
	Button* downButton;
	Button* leftButton;
	Button* rightButton;

	DirectX::XMFLOAT2 upperLeftCorner;
	DirectX::XMFLOAT2 lowerRightCorner;
	DirectX::XMFLOAT2 centerPoint;

	Button()
	{
		upButton = nullptr;
		downButton = nullptr;
		leftButton = nullptr;
		rightButton = nullptr;
	}
};