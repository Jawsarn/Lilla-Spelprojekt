#include "StaticObj.h"



StaticObj::StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box)
{
	m_typeOfObject = p_typeOfObject;
	m_box = p_box;
}

StaticObj::~StaticObj(void)
{
}

BoundingOrientedBox* StaticObj::GetBox()
{
	return &m_box;
}

ObjectType StaticObj::GetType()
{
	return m_typeOfObject;
}

