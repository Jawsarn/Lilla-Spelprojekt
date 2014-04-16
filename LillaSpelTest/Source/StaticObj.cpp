#include "StaticObj.h"


StaticObj::StaticObj(DirectX::XMFLOAT3 p_position,DirectX::XMFLOAT3 p_direction, ObjectType p_typeOfObject, BoundingOrientedBox p_box)
	:Object(p_box, p_direction, p_position)
{
	m_typeOfObject = p_typeOfObject;
}

StaticObj::~StaticObj(void)
{
}

