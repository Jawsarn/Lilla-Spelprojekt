#pragma once
#include "object.h"
class PlayerWall :
	public Object
{
private:
	DirectX::XMFLOAT3 m_color;
public:
	PlayerWall(void);
	PlayerWall(DirectX::XMFLOAT3 p_color, DirectX::XMFLOAT3 p_position);
	BoundingOrientedBox* GetBox();
	~PlayerWall(void);

	void Update(float p_dt);
};

