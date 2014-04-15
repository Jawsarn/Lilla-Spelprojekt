#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "MapNode.h"

using namespace std;
using namespace DirectX;

class MapLoader
{
public:
	MapLoader(void);
	vector<MapNode*>LoadMap(string mapName);
	~MapLoader(void);
private:
	vector<vector<XMFLOAT3>> LoadLogicalObj(string splineName);
	//vector<StaticObj*> LoadBoxes(string boxListName);

};

