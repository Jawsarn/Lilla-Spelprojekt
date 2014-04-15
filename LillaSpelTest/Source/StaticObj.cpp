#include "StaticObj.h"


StaticObj::StaticObj(DirectX::XMFLOAT3 p_position,DirectX::XMFLOAT3 p_direction, Type p_typeOfObject)
{
	m_position = p_position;
	m_direction = p_direction;
	m_box.type = p_typeOfObject;
}

StaticObj::StaticObj(void)
{

}

StaticObj::~StaticObj(void)
{
}
