#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

class Object
{
protected:
	BoundingOrientedBox m_box;
	BoundingSphere m_sphere;
	DirectX::XMFLOAT3 m_direction;	
	DirectX::XMFLOAT3 m_position;

public:
	Object();
	Object(BoundingOrientedBox p_box, XMFLOAT3 p_direction, XMFLOAT3 p_position);
	void SetBoundingSphere(BoundingSphere p_sphere);
	~Object(void);
};

