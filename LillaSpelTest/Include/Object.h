#pragma once
#include "Box.h"
#include <DirectXMath.h>

class Object
{
protected:
	Box m_box;
	DirectX::XMFLOAT3 m_direction;	
	DirectX::XMFLOAT3 m_position;

public:
	Object(void);
	~Object(void);
};

