#include "Player.h"
#include "MapLoader.h"
#include <Windows.h>


Player::Player()
{

}


Player::Player(MapNode* p_startNode, float p_startAngle, int p_playerIndex)
{
	m_state = STARTING;
	m_deltaAngle = 0;
	MapLoader t_mapLoader = MapLoader(); //might need to explicitly call constructor to get member variable set. Dunno about this one...
	m_mathHelper = MathHelper();
	m_previousUserCmd = UserCMD(p_playerIndex);
	m_currentUserCmd = UserCMD(p_playerIndex);
	//gets the corners from the maploader. Yes. This works
	vector<XMFLOAT3> t_wallBoxCorners = t_mapLoader.LoadLogicalObj("walls/firstwall/mesh.obj").at(0);
	vector <XMFLOAT3> t_playerShipBoxCorners = t_mapLoader.LoadLogicalObj("ships/pajfighter/mesh.obj").at(0);

	m_wallBoxExtents = SetBoxExtents(t_wallBoxCorners);
	m_playerShipBoxExtents = SetBoxExtents(t_playerShipBoxCorners);

	m_playerIndex = p_playerIndex;

	m_bobOffset = XMFLOAT3(0,0,0);
	m_maxBoost = 20000;
	m_maxWalls = 100;
	m_boostDecay = 100;
	m_racePos = 1;
	m_coolDownDecay = 0.03;
	m_mapNode = p_startNode;
	m_upVector = XMFLOAT3(0,1,0);

	m_maxImmortalTimer = 5;
	m_maxDeathTimer = 1;

	m_distance = 0.0f;
	m_boostMeter = 20000;//test value

	m_position = m_mapNode->m_position;
	m_direction = DirectX::XMFLOAT3(0,0,1);
	m_lastPlacedWall = nullptr;

	//speed stuff
	m_speed = 0;
	m_maxSpeed = 20;
	m_maxBoostSpeed = 60;//

	m_acceleration = 8;
	m_boostAcceleration = 25;
	m_deceleration = 16;

	//rotation stuff
	m_rotateSpeed = 0.05;
	m_angle = p_startAngle;

	//initilaize world matrix stuff
	FixUpVectorRotation(m_angle);
	FixOffsetFromCenterSpline();
	UpdateWorldMatrix();
}


Player::~Player(void)
{
}


//////THE BOSS METHOD
int Player::ProperUpdatePosition(float p_dt, UserCMD p_userCMD)
{
	int r_returnInt = 0;
	m_previousUserCmd = m_currentUserCmd;
	m_currentUserCmd = p_userCMD;
	MathHelper t_mathHelper;

	//not sure if entierly needed...
	m_direction = XMFLOAT3(0,0,1);

	if(m_state ==STARTING)
		StartupSpam();
	if(m_state == NORMAL || m_state == IMMORTAL)
	{
		Acceleration(p_dt);
		Rotation(p_dt);
		MovementAlongLogicalMap(p_dt);
		SetDirection();
		FixWorldPosition();
		UpdateCollisionBox();

		r_returnInt = WallPlacement(p_dt);
		UpdateTimers(p_dt);

	}

	return r_returnInt;
}


//////BIG FAT ARISTOCRAT METHODS
void Player::StartupSpam()
{
	if(m_previousUserCmd.aButtonPressed && !m_currentUserCmd.aButtonPressed)
	{
		m_aButtonPressedAtStart++;
	}
}

void Player::Acceleration(float p_dt)
{
	////Acceleration
	//boost acceleration
	if(m_currentUserCmd.rightBumberPressed && m_boostMeter>0)
	{
		//check if max boost speed is attained, otherwise accelerate
		if(m_maxBoostSpeed>m_speed)
		{
			m_speed += p_dt*m_boostAcceleration;
		}
		else
		{
			//if you've reached the max boost speed, you stay there
			m_speed = m_maxBoostSpeed;
		}
		//lower remaining boost
		m_boostMeter -= m_boostDecay;
	}
	//ordinary acceleration
	else
	{
		if(m_maxSpeed >m_speed)
		{
			m_speed+=p_dt*m_acceleration;
		}
		//decelerate if you're above ordinary max speed
		else
		{
			m_speed-=p_dt*m_deceleration;
		}
	}
	if(m_currentUserCmd.xButtonPressed)
		m_speed+= 2*m_boostAcceleration*p_dt;
}

void Player::Rotation(float p_dt)
{
	m_deltaAngle = 0;
	m_deltaAngle = m_currentUserCmd.Joystick.x*m_rotateSpeed;
	m_angle += m_deltaAngle;
}

void Player::MovementAlongLogicalMap(float p_dt)
{
	////movement along logical map
	//adds distance from the current node based on speed and time since last update
	m_distance+=m_speed*p_dt;
	//Checks of distance exceeds distance to new node. In other words, if the player "overshoots" the next node
	while(m_distance >= m_mathHelper.Abs(m_mapNode->m_normal))
	{
		float t_remainingDistance = m_distance - m_mathHelper.Abs(m_mapNode->m_normal);
		m_distance = t_remainingDistance;
		m_mapNode = m_mapNode->m_nextNode;
	}
	//Moves the position along the normal of current node with distance
	XMFLOAT3 t_nodeNormalDirection = m_mathHelper.Normalize(m_mapNode->m_normal);
	XMFLOAT3 t_vectorToMove = m_mathHelper.FloatMultiVec(m_distance, t_nodeNormalDirection);

	m_position = m_mathHelper.VecAddVec(m_mapNode->m_position, t_vectorToMove);
	//now following middle spline
}

void Player::SetDirection()
{
	///SETS DIRECTION OF PLAYER
	//interpolate normals between current and previous
	float t_interpolation;
	t_interpolation = m_distance/m_mathHelper.Abs(m_mapNode->m_normal);
	XMFLOAT3 t_frontNormalComponent = m_mathHelper.FloatMultiVec(t_interpolation, m_mathHelper.Normalize(m_mapNode->m_nextNode->m_normal));
	XMFLOAT3 t_currentNormalComponent = m_mathHelper.FloatMultiVec(1-t_interpolation,m_mathHelper.Normalize(m_mapNode->m_normal));
	m_direction = m_mathHelper.Normalize( m_mathHelper.VecAddVec(t_frontNormalComponent, t_currentNormalComponent));

	//now looking along the interpolated normal between current node and next node 
}

void Player::FixWorldPosition()
{
	////Fix from logical map to actual world position and orientation
	FixUpVectorRotation(m_angle);
	//now rotating around the normal

	FixOffsetFromCenterSpline();
	//now offset from the center, following the tube edge

	BobOffset();
	//now offset a tiny bit from "Actual position" depending on speed

	UpdateWorldMatrix();
	//Matrix now updates. Ready to be grabbed from the gamescreen
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
	m_box.Extents = m_playerShipBoxExtents; //////TEMPEXTENTSLOL
	m_box.Orientation = t_quarternion;


}

int Player::WallPlacement(float p_dt)
{
	m_coolDown -= p_dt;
	if(m_currentUserCmd.rightTriggerPressed && m_coolDown <=0 && m_wallMeter < 1)
	{
		m_wallMeter-=1;
		PlaceWall();
		m_coolDown = 1;
		return  1;
	}
	return 0;

}

void Player::UpdateTimers(float p_dt)
{
	////immortal and death timers
	m_immortalTimer-=p_dt;
	if(m_immortalTimer<0)
		m_state = NORMAL;
	m_deathTimer -=p_dt;

}





/////PUNY PEASANT SLAVE METHODS


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
	m_lastPlacedWall = new PlayerWall(XMFLOAT3(0,0,1), &m_position, &m_direction , &m_upVector, m_playerIndex, m_wallBoxExtents);
	m_placedWalls.push_back(m_lastPlacedWall);
	m_mapNode->AddWall(m_lastPlacedWall);
}

void Player::FixUpVectorRotation(float p_angle)
{
	MathHelper t_mathHelper = MathHelper();

	//Rotates the radius vector with angle around the normal/direction. Sets it to the up vector. Azooka made uber-hax.
	XMMATRIX t_rotationmatrix = XMMatrixRotationAxis(XMLoadFloat3(&t_mathHelper.Normalize( m_mapNode->m_normal)), p_angle);
	XMVECTOR t_upVector = XMLoadFloat3(&m_mapNode->m_radiusVector);
	t_upVector = XMVector3Transform(t_upVector, t_rotationmatrix);
	XMStoreFloat3(&m_upVector, t_upVector);
	m_upVector = t_mathHelper.Normalize(m_upVector);
}

void Player::FixOffsetFromCenterSpline()
{
	m_position = m_mathHelper.VecAddVec(m_position, m_mathHelper.FloatMultiVec(-m_mapNode->m_radius+(m_mapNode->m_radius/4), m_upVector)); 
}

void Player::BobOffset()
{
	//declaring offset variables
	float t_bobX, t_bobY, t_bobZ;

	uniform_real_distribution<float> distribution(-0.05, 0.05);
	t_bobX = distribution(m_randomGenerator);
	t_bobY = distribution(m_randomGenerator);
	t_bobZ = distribution(m_randomGenerator);

	m_bobOffset = m_mathHelper.FloatMultiVec(m_speed/m_maxBoostSpeed/3,XMFLOAT3(t_bobX, t_bobY, t_bobZ));

}

void Player::UpdateWorldMatrix()
{
	//change to properly calibrate the camera offset (don't wanna have dat camera clip thru he wall)
	float t_cameraUpTrailDistance = 1;
	float t_cameraTargetTrailDistance = 5;
	//Vehicle Variables
	XMFLOAT3 t_position = XMFLOAT3(m_position.x, m_position.y, m_position.z);


	XMVECTOR t_vehicleEyeVector = XMLoadFloat3(&t_position);
	XMVECTOR t_vehicleTargetVector = XMLoadFloat3(&m_direction);
	XMVECTOR t_vehicleUpVector = XMLoadFloat3(&m_upVector);

	XMVECTOR t_cameraEyeVector = t_vehicleEyeVector+t_vehicleUpVector*t_cameraUpTrailDistance+t_cameraTargetTrailDistance*t_vehicleTargetVector*-1;
	XMVECTOR t_cameraUpVector = t_vehicleUpVector;
	XMVECTOR t_cameraTargetVector = t_vehicleEyeVector;

	////VEHICLE TILT
	//rotate along target vector
	XMMATRIX t_directionRotationMatrixTarget = XMMatrixRotationAxis(t_vehicleTargetVector,m_deltaAngle*3);					//////////////////////////MAKE SURE YOU CHANGE THIS HARDCODED 10 CRAP////////////////
	t_vehicleUpVector = XMVector3Transform(t_vehicleUpVector, t_directionRotationMatrixTarget);
	t_vehicleUpVector = XMVector3Normalize(t_vehicleUpVector);

	//roate along new up vector
	XMMATRIX t_directionRotationMatrixUp = XMMatrixRotationAxis(t_vehicleUpVector, m_deltaAngle*10);					//////////////////////////MAKE SURE YOU CHANGE THIS HARDCODED 10 CRAP////////////////
	t_vehicleTargetVector = XMVector3Transform(t_vehicleTargetVector, t_directionRotationMatrixUp);
	t_vehicleTargetVector = XMVector3Normalize(t_vehicleTargetVector);



	XMStoreFloat4x4(&m_cameraMatrix , XMMatrixLookAtLH(t_cameraEyeVector, t_cameraTargetVector, t_vehicleUpVector));


	XMVECTOR t_bobOffsetVector = XMLoadFloat3(&m_bobOffset);
	XMStoreFloat4x4( &m_worldMatrix, XMMatrixLookToLH(t_vehicleEyeVector+t_bobOffsetVector, t_vehicleTargetVector, t_vehicleUpVector));

	//store direction of car for wall placement
	XMStoreFloat3(&m_direction, t_vehicleTargetVector);




}
void Player::BumpedIntoPlayer(XMFLOAT3 p_force)
{
	MathHelper t_mathHelper = MathHelper();
	m_speed -= t_mathHelper.Abs(t_mathHelper.FloatMultiVec(t_mathHelper.Abs(t_mathHelper.VecAddVec(m_direction, p_force)),m_mapNode->m_normal)); //VETTEFANOMDETT
	//Angle ska bli p_force projicerad på m_direction cross m_upvector och sen absolutbelopp på den vectorn
}

XMFLOAT3 Player::SetBoxExtents(vector<XMFLOAT3> p_corners)
{
	float t_maxX = 0;
	float t_maxY = 0;
	float t_maxZ = 0;

	float t_minX = 0;
	float t_minY = 0;
	float t_minZ = 0;
	for (int i = 0; i < p_corners.size(); i++)
	{
		//gets max and min x
		if(p_corners[i].x > t_maxX)
			t_maxX = p_corners[i].x;
		else if(p_corners[i].x < t_minX)
			t_minX = p_corners[i].x;

		//gets max and min y
		if(p_corners[i].x > t_maxY)
			t_maxY=p_corners[i].x;
		else if(p_corners[i].x < t_minY)
			t_minY = p_corners[i].x;

		//gets max and min Z
		if(p_corners[i].x > t_maxZ)
			t_maxZ = p_corners[i].x ;
		else if(p_corners[i].x < t_minZ)
			t_minZ=p_corners[i].x ;
	}

	//checks if absolute value of min values is greater than max values (really unnecessary if models are symetric, but can TA really be trusted?
	if(t_maxX < -t_minX)
		t_maxX = -t_minX;

	if(t_maxY < -t_minY)
		t_maxY = -t_minY;

	if(t_maxZ < -t_minZ)
		t_maxZ = -t_minZ;

	return XMFLOAT3(t_maxX, t_maxY, t_maxZ);
}





/////MESSENGERS
//Accessors
XMMATRIX Player::GetWorldMatrix()
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_worldMatrix));
}

XMMATRIX Player::GetCamMatrix()
{
	return XMLoadFloat4x4( &m_cameraMatrix);
}
MapNode* Player::GetCurrentMapNode()
{
	return m_mapNode;
}

BoundingOrientedBox* Player::GetCollisionBox()
{
	return &m_box;
}

PlayerWall* Player::GetLastPlacedWall()
{
	return m_lastPlacedWall;	
}

std::vector<BoundingOrientedBox*> Player::GetWallsToCheck()
{
	std::vector<BoundingOrientedBox*> r_return;
	return r_return;
}

XMFLOAT3 Player::GetPos()
{
	return m_position;
}

XMFLOAT3 Player::GetDirection()
{
	return m_direction;
}

float Player::GetPlayerBoost()
{
	return m_boostMeter;
}



float Player::GetDistanceTraveled()
{
	float t_distance = (float)m_mapNode->m_Index;
	t_distance += m_distance/100; //SÅ att du kan skilja på positioner även om fler spelare är i samma mapnode
	return t_distance;
}



int Player::GetRacePosition()
{
	return m_racePos;
}

float Player::GetHudBoosterInfo()
{
	//apparently wants 0 to be alot of boost, and 1 to be empty
	return 1-(m_boostMeter/m_maxBoost);
}

float Player::GetHudWallInfo()
{
	//apparently wants 0 to be alot of walls, and 1 to be empty
	return 1-(m_maxWalls/m_wallMeter);
}

bool Player::GetImmortal()
{
	return(m_state == IMMORTAL);
}



int Player::GetPlayerIndex()
{
	return m_playerIndex;
}

int Player::GetNrOfAPressedAtStart()
{
	return m_aButtonPressedAtStart;
}

//Modifiers
void Player::Die()
{
	m_speed = 0;
	m_immortalTimer = m_maxImmortalTimer;
	m_state = IMMORTAL;
}

void Player::SetPlayerRacePosition(int p_pos)
{
	m_racePos = p_pos;
}

void Player::SetPlayerBoost(float p_boost)
{
	m_boostMeter = p_boost;
}
void Player::Start()
{
	m_speed = (float)(m_aButtonPressedAtStart)/2;
	m_state = NORMAL;
}
////scrap

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
