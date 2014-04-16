#include "Object.h"


Object::Object()
{

}

Object::Object(BoundingOrientedBox p_box, XMFLOAT3 p_direction, XMFLOAT3 p_position)
{
	m_box = p_box;
	m_direction = p_direction;
	m_position = p_position;
}


Object::~Object(void)
{
}
