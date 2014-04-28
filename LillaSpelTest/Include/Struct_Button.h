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

	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 centerPoint;

	unsigned int buttonHandle;
	Button()
	{
		upButton = nullptr;
		downButton = nullptr;
		leftButton = nullptr;
		rightButton = nullptr;
	}
};