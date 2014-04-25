#include "Player.h"
#include "MathHelper.h"


Player::Player()
{

}


Player::Player(MapNode* p_startNode, float p_startAngle)
{
	m_mapNode = p_startNode;
	m_upVector = XMFLOAT3(0,1,0);
	m_angle = p_startAngle;
	m_distance = 0.0f;
	m_speed = 30;
	m_position = m_mapNode->m_position;
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
	////silly boost thingy
	if(p_userCMD.aButtonPressed)
		m_speed = 200;
	else if (p_userCMD.bButtonPressed)
		m_speed = 30;
	else if(p_userCMD.xButtonPressed)
		m_speed = -30;
	else 
		m_speed = 0;

	MathHelper t_mathHelper;
	//adds distance from the current node based on speed and time since last update
	m_distance+=m_speed*p_dt;
	//Checks of distance exceeds distance to new node. In other words, if the player "overshoots" the next node
	while(m_distance >= t_mathHelper.Abs(m_mapNode->m_normal))
	{
		float t_remainingDistance = m_distance - t_mathHelper.Abs(m_mapNode->m_normal);
		m_distance = t_remainingDistance;
		m_mapNode = m_mapNode->m_nextNode;
	}
	//Moves the position along the normal of current node with distance
	XMFLOAT3 t_nodeNormalDirection = t_mathHelper.Normalize(m_mapNode->m_normal);
	XMFLOAT3 t_vectorToMove = t_mathHelper.FloatMultiVec(m_distance, t_nodeNormalDirection);

	m_position = t_mathHelper.VecAddVec(m_mapNode->m_position, t_vectorToMove);
	
	



	//interpolate normals between current and previous
	float t_interpolation;
	t_interpolation = m_distance/t_mathHelper.Abs(m_mapNode->m_normal);
	XMFLOAT3 t_frontNormalComponent = t_mathHelper.FloatMultiVec(t_interpolation, t_mathHelper.Normalize(m_mapNode->m_nextNode->m_normal));
	XMFLOAT3 t_currentNormalComponent = t_mathHelper.FloatMultiVec(1-t_interpolation,t_mathHelper.Normalize(m_mapNode->m_normal));
	m_direction = t_mathHelper.Normalize( t_mathHelper.VecAddVec(t_frontNormalComponent, t_currentNormalComponent));
	//m_direction = m_mapNode->m_normal;

	
	FixUpVectorRotation(p_userCMD.Joystick.x/20/*m_angle*/);
	FixOffsetFromCenterSpline();
	UpdateCollisionBox();

}

XMMATRIX Player::GetWorldMatrix()
{
	XMFLOAT3 t_position = XMFLOAT3(m_position.x, m_position.y, m_position.z);
	XMVECTOR t_eye = XMLoadFloat3(&t_position);
	XMVECTOR t_target = XMLoadFloat3(&m_direction);
	XMVECTOR t_up = XMLoadFloat3(&m_upVector);

	XMMATRIX r_worldMatrix = XMMatrixLookToLH(t_eye, t_target, t_up);

	return r_worldMatrix;

}

MapNode* Player::GetCurrentMapNode()
{
	return m_mapNode;
}

BoundingOrientedBox* Player::GetCollisionBox()
{
	return &m_box;
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

void Player::FixUpVectorRotation(float p_angle)
{
	MathHelper t_mathHelper = MathHelper();

	XMMATRIX t_rotationmatrix = XMMatrixRotationAxis(XMLoadFloat3(&t_mathHelper.Normalize( m_mapNode->m_normal)), p_angle);
	XMVECTOR t_upVector = XMLoadFloat3(&m_upVector);
	t_upVector = XMVector3Transform(t_upVector, t_rotationmatrix);
	XMStoreFloat3(&m_upVector, t_upVector);
}

void Player::FixOffsetFromCenterSpline()
{
	MathHelper t_mathHelper = MathHelper();

	m_position = t_mathHelper.VecAddVec(m_position, t_mathHelper.FloatMultiVec(-m_mapNode->m_radius+(m_mapNode->m_radius/4), m_upVector)); 
}

void Player::UpdateCollisionBox()
{
	MathHelper t_mathHelper = MathHelper();
	//XMFLOAT3 t_vector = t_mathHelper.CrossProduct(m_direction, m_upVector); kanske inte behövs
	XMFLOAT4 t_quarternion = XMFLOAT4(0,0,0,1);
	XMMATRIX t_boxOrientationMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_direction),XMLoadFloat3(&m_upVector)); //matrixyo
	XMVECTOR t_boxOrientationVector = XMLoadFloat4(&t_quarternion);
	t_boxOrientationVector = XMVector4Transform(t_boxOrientationVector, t_boxOrientationMatrix);
	t_boxOrientationVector = XMVector4Normalize(t_boxOrientationVector);
	XMStoreFloat4(&t_quarternion, t_boxOrientationVector);

	m_box.Center = m_position;
	m_box.Extents = XMFLOAT3(10,10,10); //////TEMPEXTENTSLOL
	m_box.Orientation = t_quarternion;


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


void Player::BumpedIntoPlayer(XMFLOAT3 p_force)
{
	MathHelper t_mathHelper = MathHelper();
	m_speed -= t_mathHelper.Abs(t_mathHelper.FloatMultiVec(t_mathHelper.Abs(t_mathHelper.VecAddVec(m_direction, p_force)),m_mapNode->m_normal)); //VETTEFANOMDETTA
	//Angle ska bli p_force projicerad på m_direction cross m_upvector och sen absolutbelopp på den vectorn

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