#include "Player.h"
#include "MathHelper.h"


Player::Player()
{
	m_speed = 30;
	m_position = DirectX::XMFLOAT3(0,0,0);
	m_direction = DirectX::XMFLOAT3(0,0,0);
}


Player::Player(MapNode* p_startNode, float p_startAngle)
{
	m_mapNode = p_startNode;
	m_angle = p_startAngle;
}


Player::~Player(void)
{
}

void Player::Update(float p_dt, UserCMD userCMD)
{
	switch (m_state)
	{
	case NORMAL:
		break;
	case DEAD:
		m_deathTimer -= p_dt;
		if (m_deathTimer <=0)
		{
			ChangeState(IMMORTAL);
		}
		break;
	case IMMORTAL:
		m_immortalTimer -= p_dt;
		if (m_immortalTimer <= 0)
		{
			ChangeState(NORMAL);
		}
		break;
	default:
		break;
	}
}
void Player::UpdatePosition(float p_dt, UserCMD p_userCMD)
{
	////free moving////
	MathHelper t_mathHelper;
	m_direction.x += p_userCMD.Joystick.x;
	m_direction.y += p_userCMD.Joystick.y;
	m_direction = t_mathHelper.Normalize(m_direction);

	m_position = t_mathHelper.FloatMultiVec(p_dt,t_mathHelper.FloatMultiVec(m_speed,m_direction));
}

std::vector<BoundingOrientedBox*> Player::GetWallsToCheck()
{
	std::vector<BoundingOrientedBox*> r_return;
	return r_return;
}

void Player::ChangeState(PlayerState p_state)
{
	m_state = p_state;
	switch (m_state)
	{
	case NORMAL:
		break;
	case DEAD:
		m_deathTimer =  DEATH_TIME;
		break;
	case IMMORTAL:
		m_immortalTimer = IMMORTAL_TIME;
		break;
	default:
		break;
	}
}

void Player::PlaceWall()
{
}

void Player::UpdateCollisionBox()
{
}

void Player::CheckMapNode()
{
	////klurigjävlakod;
}






/// Gets yo
XMFLOAT3 Player::GetPos()
{
	return m_position;
}

XMFLOAT3 Player::GetDirection()
{
	return m_direction;
}