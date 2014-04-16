#pragma once
#include "object.h"

enum ObjectType{HOLE, WALL};

class StaticObj :
	public Object
{
private:
	ObjectType m_typeOfObject;
	
public:

	StaticObj(DirectX::XMFLOAT3 p_position, DirectX::XMFLOAT3 p_direction, ObjectType p_typeOfObject, BoundingOrientedBox p_box);
	StaticObj(void);
	~StaticObj(void);

};

