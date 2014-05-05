#pragma once
#include "object.h"
using namespace DirectX;
class PlayerWall :
	public Object
{
private:
	DirectX::XMFLOAT3 m_color;
	XMFLOAT4X4 m_worldMatrix;
public:
	PlayerWall(void);
	PlayerWall(XMFLOAT3 p_color, XMFLOAT3* p_wallPos, XMFLOAT3* p_wallDir, XMFLOAT3* p_wallUp);
	BoundingOrientedBox* GetBox();
	BoundingSphere* GetSphere();
	~PlayerWall(void);
	unsigned int m_wallIndex;
	void Update(float p_dt);

	unsigned int GetWallIndex();
	XMMATRIX GetWorldMatrix();
};

