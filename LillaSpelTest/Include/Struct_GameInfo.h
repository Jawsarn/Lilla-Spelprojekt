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
	int shipModell[4];
	int playerColor[4];
	bool playerOnline[4];
	int resolution;
	int map;

	GameInfo()
	{
		for (int i = 0; i < 4; i++)
		{
			playerOnline[i] = false;
		}
	}
};