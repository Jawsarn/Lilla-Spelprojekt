#include "StaticObj.h"



StaticObj::StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box)
{
	m_typeOfObject = p_typeOfObject;
	m_box = p_box;
	//XMMATRIX testRotation = XMMatrixRotationQuaternion(XMLoadFloat4(&m_box.Orientation));
	//XMVECTOR 
	//XMMATRIX testTransformation = XMMatrixTransformation()
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

