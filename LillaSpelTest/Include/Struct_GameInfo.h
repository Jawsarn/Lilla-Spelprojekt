#pragma once
#include <string>
#define NUMBER_OF_MODELLS 5
#define NUMBER_OF_COLORS 5

struct PlayerStartInfo
{
	int modell;
	int color;
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