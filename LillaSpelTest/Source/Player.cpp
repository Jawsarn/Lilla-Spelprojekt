#include "Player.h"


Player::Player(void)
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
	case Normal:
		break;
	case Dead:
		m_deathTimer -= p_dt;
		if (m_deathTimer <=0)
		{
			ChangeState(Immortal);
		}
		break;
	case Immortal:
		m_immortalTimer -= p_dt;
		if (m_immortalTimer <= 0)
		{
			ChangeState(Normal);
		}
		break;
	default:
		break;
	}
}

std::vector<Box*> Player::GetWallsToCheck()
{
}

void Player::ChangeState(PlayerState p_state)
{
	m_state = p_state;
	switch (m_state)
	{
	case Normal:
		break;
	case Dead:
		m_deathTimer =  DEATH_TIME;
		break;
	case Immortal:
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
