#include "Player.h"



Player::Player()
{

}


Player::Player(MapNode* p_startNode, float p_startAngle)
{
	m_mapNode = p_startNode;
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
