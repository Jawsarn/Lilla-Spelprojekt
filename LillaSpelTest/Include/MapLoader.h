#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "MathHelper.h"
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
	//variables
	vector<MapNode*> m_logicalMap;
	vector<StaticObj*> m_boxes;
	MathHelper m_mathHelper;


	//methods
	vector<vector<XMFLOAT3>> LoadLogicalObj(string p_objName);
	void LoadNodes(vector<XMFLOAT3>* p_centerPositions, vector<XMFLOAT3>* p_edgePositions);
	void LoadBoxes(vector<vector<XMFLOAT3>>* p_boxCornerPositions, ObjectType p_objectType); 
	void AssignBoxesToNodes();
	XMVECTOR GetUpVector(XMFLOAT3 p_normal, XMFLOAT3 p_radius);

};

