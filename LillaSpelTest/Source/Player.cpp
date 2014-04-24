#include "Player.h"
#include "MathHelper.h"


Player::Player()
{

}


Player::Player(MapNode* p_startNode, float p_startAngle)
{
	m_mapNode = p_startNode;
	m_angle = p_startAngle;
	m_distance = 0.0f;
	m_speed = 3;
	m_position = DirectX::XMFLOAT3(0,0,0);
	m_direction = DirectX::XMFLOAT3(0,0,0);
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

void Player::ProperUpdatePosition(float p_dt, UserCMD p_userCMD)
{
	MathHelper t_mathHelper;
	//adds distance from the current node based on speed and time since last update
	m_distance+=m_speed*p_dt;
	//Checks of distance exceeds distance to new node. In other words, if the player "overshoots" the next node
	while(m_distance >= t_mathHelper.Abs(m_mapNode->m_normal))
	{
		m_distance -= t_mathHelper.Abs(m_mapNode->m_normal);
		m_mapNode = m_mapNode->m_nextNode;
	}
	//Moves the position along the normal of current node with distance
	XMFLOAT3 t_nodeNormalDirection = t_mathHelper.Normalize(m_mapNode->m_normal);
	XMFLOAT3 t_vectorToMove = t_mathHelper.FloatMultiVec(m_distance, t_nodeNormalDirection);
	m_position = t_mathHelper.VecAddVec(m_mapNode->m_position, t_vectorToMove);

}

XMMATRIX Player::GetWorldMatrix()
{
	XMVECTOR t_eye = XMLoadFloat3(&m_position);
	XMVECTOR t_target = XMLoadFloat3(&XMFLOAT3(0,0,1));
	XMVECTOR t_up = XMLoadFloat3(&XMFLOAT3(0,1,0));

	XMMATRIX r_worldMatrix = XMMatrixLookAtLH(t_eye, t_target, t_up);

	return r_worldMatrix;

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

void Player::UpdateMapNode() //Uppdaterar logisk playerpos(m_logicalPosition) och currMapnode
{
	int t_WhileBreak = 0;
	XMFLOAT3 t_vec;
	MathHelper t_mathHelp = MathHelper();
	float t_distFromPlayerToCurrNode = t_mathHelp.Abs(t_mathHelp.VecSubVec(m_mapNode->m_position, m_logicalPosition)); //Skapar en vector mellan playerpos å currentnodepos och tar absolutvärde
	while(t_WhileBreak = 0)
	{
		if(t_distFromPlayerToCurrNode > t_mathHelp.Abs(m_mapNode->m_normal)) //Kolla om den skapade vectorn är längre än vad normalen(Vectorn mellan node och node->next
		{
			t_distFromPlayerToCurrNode -= t_mathHelp.Abs(m_mapNode->m_normal); //Tar bort den "travelade" längden 
			m_mapNode = m_mapNode->m_nextNode;									//Byta currmapnode
			t_vec = t_mathHelp.FloatMultiVec(t_distFromPlayerToCurrNode, t_mathHelp.Normalize(m_mapNode->m_normal));  //Skapa ny vector solm har samma uppgift å egenskaper som den första
			m_logicalPosition = t_vec;																							//Uppdatera playerpos
			//t_distFromPlayerToCurrNode = t_mathHelp.Abs(t_vec);															//Behövs inte 

		}
		else
		{
			t_WhileBreak = 1;
		}
	}
	//Med Antagande att position är uppdaterad redan
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