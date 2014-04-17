#pragma once
#include <string>
#include <DirectXMath.h>
#define NUMBER_OF_MODELLS 5

struct PlayerStartInfo
{
	int modell;
	DirectX::XMFLOAT3 color;
};

struct GameInfo
{
	bool playerOnline[4];
	int resolution;
	std::string mapName;

	GameInfo()
	{
		for (int i = 0; i < 4; i++)
		{
			playerOnline[i] = false;
		}
	}
};