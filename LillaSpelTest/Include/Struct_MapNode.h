#pragma once
#include <vector>
#include "StaticObj.h"
#include "PlayerWall.h"

using namespace std;
using namespace DirectX;
struct MapNode
{
	MapNode* m_previousNode;
	MapNode* m_nextNode;
	std::vector<StaticObj*>m_staticObjs;
	std::vector<PlayerWall*>m_playerWalls;
	DirectX::XMFLOAT3 m_normal;
	DirectX::XMFLOAT3 m_position;
	XMFLOAT3 m_radiusVector;
	float m_radius;
	int m_Index;
	MapNode()
	{
		m_previousNode = nullptr;
		m_nextNode = nullptr;
	}
	void AddWall(PlayerWall* p_wall)
	{
		m_playerWalls.push_back(p_wall);
	}
};