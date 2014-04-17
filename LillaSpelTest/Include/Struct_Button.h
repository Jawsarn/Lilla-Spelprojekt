#pragma once
#include <string>
#include <DirectXMath.h>

struct Button
{
	Button* upButton;
	Button* downButton;
	Button* leftButton;
	Button* rightButton;

	DirectX::XMFLOAT2 upperLeftCorner;
	DirectX::XMFLOAT2 lowerRightCorner;
	DirectX::XMFLOAT2 centerPoint;

	std::string buttonName;
	Button()
	{
		upButton = nullptr;
		downButton = nullptr;
		leftButton = nullptr;
		rightButton = nullptr;
	}
};