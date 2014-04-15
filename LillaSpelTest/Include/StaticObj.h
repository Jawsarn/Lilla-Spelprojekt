#pragma once
#include "object.h"
class StaticObj :
	public Object
{
public:
	StaticObj(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT3 p_direction, Type p_typeOfObject);
	StaticObj(void);
	~StaticObj(void);

};

