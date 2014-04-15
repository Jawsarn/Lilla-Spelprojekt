#include "PlayerWall.h"


PlayerWall::PlayerWall(void)
{

}

PlayerWall::PlayerWall(DirectX::XMFLOAT3 p_color,DirectX::XMFLOAT3 p_position)
{
	m_color = p_color;
	m_position = p_position;
}


PlayerWall::~PlayerWall(void)
{
}
