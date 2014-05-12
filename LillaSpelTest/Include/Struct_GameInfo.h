#pragma once
#include <string>


struct PlayerStartInfo
{
	int modell;
	int color;
};

struct GameInfo
{
	int shipModell[4];
	int playerColor[4];
	std::string tauntSound[4];
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