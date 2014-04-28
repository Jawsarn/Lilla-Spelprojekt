#pragma once
#include "object.h"
using namespace DirectX;
class PlayerWall :
	public Object
{
private:
	DirectX::XMFLOAT3 m_color;
public:
	PlayerWall(void);
	PlayerWall(XMFLOAT3 p_color, const XMMATRIX &p_wallWorldMatrix, XMFLOAT3 p_playerPosition);
	BoundingOrientedBox* GetBox();
	XMMATRIX m_worldMatrix;
	~PlayerWall(void);
	unsigned int m_wallIndex;
	void Update(float p_dt);

	unsigned int GetWallIndex();
	XMMATRIX GetWorldMatrix();
};

