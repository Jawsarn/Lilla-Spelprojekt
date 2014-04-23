#include "StaticObj.h"



StaticObj::StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box)
{
	m_typeOfObject = p_typeOfObject;
	m_box = p_box;
}

StaticObj::~StaticObj(void)
{
}

