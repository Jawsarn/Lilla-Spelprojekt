#include "StaticObj.h"



StaticObj::StaticObj(ObjectType p_typeOfObject, BoundingOrientedBox p_box)
{
	m_typeOfObject = p_typeOfObject;
	m_box = p_box;

	XMVECTOR t_scalingVector = XMLoadFloat3(&m_box.Center);
	XMVECTOR t_scalingFactor = XMLoadFloat3(&XMFLOAT3(1,1,1));
	XMVECTOR t_rotationOrigin = t_scalingVector;
	XMVECTOR t_orientation = XMLoadFloat4(&m_box.Orientation);
	XMVECTOR t_position = XMLoadFloat3(&m_box.Center);

	XMMATRIX testTransformation = XMMatrixTransformation(t_scalingVector,t_orientation, t_scalingFactor, t_rotationOrigin,t_orientation, t_position);
	//m_box.Transform(m_box, XMMatrixInverse(nullptr, testTransformation));



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

