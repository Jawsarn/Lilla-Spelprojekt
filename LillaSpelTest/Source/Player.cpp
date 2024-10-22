#include "Player.h"
#include "MapLoader.h"
#include <Windows.h>


Player::Player()
{

}


Player::Player(MapNode* p_startNode, float p_startAngle, int p_playerIndex)
{


	////MACRO STUFF
	m_state = STARTING;
	m_mathHelper = MathHelper();
	m_previousUserCmd = UserCMD(p_playerIndex);
	m_currentUserCmd = UserCMD(p_playerIndex);
	m_playerIndex = p_playerIndex;
	m_racePos = 1;
	m_mapNode = p_startNode;
	m_position = m_mapNode->m_position;
	m_angle = p_startAngle;

	//collision box stuff
	MapLoader t_mapLoader = MapLoader(); //might need to explicitly call constructor to get member variable set. Dunno about this one...
	//gets the corners from the map loader. Yes. This works
	vector<XMFLOAT3> t_wallBoxCorners = t_mapLoader.LoadLogicalObj("walls/firstwall/mesh.obj").at(0);
	vector <XMFLOAT3> t_playerShipBoxCorners = t_mapLoader.LoadLogicalObj("ships/pajfighter/mesh.obj").at(0);



	m_wallBoxExtents = SetBoxExtents(t_wallBoxCorners);
	m_playerShipBoxExtents = SetBoxExtents(t_playerShipBoxCorners);

	//m_wallBoxExtents.y *= 0.1;

	////VARIABLE INITIALIZATION (not relevant for game balancing)
	m_currentAngle = 0;
	m_wallMeter = 10;
	m_coolDown = 0;
	m_aButtonPressedAtStart = 0;
	m_bobOffset = XMFLOAT3(0, 0, 0); //I think this makes the worldmatrix fucked in initialization
	m_up = XMFLOAT3(0, 1, 0);
	m_distance = 0.0f;
	m_boostMeter = 0;//test value
	m_direction = DirectX::XMFLOAT3(0, 0, 1);
	m_lastPlacedWall = nullptr;
	m_speed = 0;
	m_cameraAngle = 0;
	m_deltaAngle = 0;
	m_previousAngle = 0;
	m_hasWon = false;
	m_gravityShifting = false;
	m_gravityShiftProgress = 0;
	m_collisionAfterMath = false;
	m_unmodifiedTarget = XMFLOAT3(0,0,1);
	m_unmodifiedUp = XMFLOAT3(0,1,0);
	m_bobTimer = 0;
	m_collisionAfterSpeed = 0;
	m_finishProgress = 0;
	m_finalDirection = 1;
	m_finishSpeed = 0;
	m_finishAngle = 0;
	m_finishSlideSpeed = 0;
	m_lap = 1;

	m_radius = p_startNode->m_radius;
	m_changedNode = false;

	////BALANCING VARIABLES

	//speed
	m_maxBoost = 5;//seconds you can boost whilst at max value
	m_boostGain = 1.5;//prolly not gonna be used

	m_maxSpeed = 25;
	m_maxBoostSpeed = 50;

	m_acceleration = 10;
	m_boostAcceleration = 30;
	m_deceleration = 7;
	m_break = 20;//static break force
	m_breakCoefficient = 1;//coefficient that breaks depending on your current speed
	m_minSpeed = 7;
	m_boostFromPad = 5000;//testValue
	m_megaBoostDecelerationCoefficient = 1.5;//used for when you're going stupidly fast (usually as a consequence of boostFromPad

	m_starUpSpamSpeed=7;

	//how quickly you rotate
	m_rotateSpeed = 0.20/pow(m_radius, 0.8);
	m_dampShipRotation = 0.03;

	//wall stuff
	m_maxWalls = 10;
	m_wallGain = 0.7;
	m_maxCooldown = 0.2;

	//timers
	m_maxImmortalTimer = 5;
	m_maxDeathTimer = 1;//currently unused
	m_collisionAfterMathTimer = 0;

	//the speed at which camera follows the ship when turning
	m_cameraFollowSpeed = 0.0005;

	//how far behind the vehicle the camera is 
	m_cameraTrailDistanceTarget = 8;
	//how high above the vehicle the camera is
	m_cameraTrailDistanceUp = 0.7;
	//how far to the right/left the camera will pan (used during finish)
	m_cameraTrailDistanceRight = 1.7; //probably shouldn't be less due to clipping


	m_gravityShiftCameraMoveSpeed = 1.5;
	m_gravityShiftSpeed = 0.9;

	m_deathShakeMaxIntensity = 8;//reversed intensity: higher number means lower intensity. Because logic
	m_deathShakeIntensityDrop = 4;

	//side bump
	m_bumpIntensity = 5; //set low for big-assed bump
	m_baseBumpIntensity = 1; //set high for big-assed bump
	//front bump
	m_targetBumpIntensity = 0.3;

	//Shockwave
	m_angleShockwavePower = 0.01;//set low for minor shockwave angle
	m_speedShockwavePower = 0.5;
	m_bobFrequency = 1;
	m_bobIntensity = 0.1;


	m_abilityCooldown = 3;
	m_shockWaveCooldown = 2;
	m_gravityShiftCooldown = 5;


	m_finishCameraFollowSpeed = 4;
	//min and max angles in radians
	m_maxFinishAngle = 3.1415;
	m_minFinishAngle = 3.1415/2;

	m_finishSlideSpeedCoefficient = 0.01;
	m_isDrawn =true;
	m_vehicleHoverDistance = 0.2*m_radius;

	////FINAL WORLD MATRIX INITIALIZATION
	SetDirection();
	FixUpVectorRotation(m_angle);
	FixOffsetFromCenterSpline();
	UpdateWorldMatrix();
	m_usedAbilityMaxCoolDown = m_abilityCooldown;
}


Player::~Player(void)
{
	CleanUp();
}

void Player::CleanUp()
{
	//loopa igenom hela listan � ta bort objecten som pekarna i listan pekar p�

	for (int i = 0; i < m_placedWalls.size(); i++)
	{
		delete m_placedWalls[i];
		m_placedWalls[i] = nullptr;
	}
}


//////THE BOSS METHOD
int Player::ProperUpdatePosition(float p_dt, UserCMD p_userCMD)
{
	m_direction = XMFLOAT3(0, 0, 1);//not sure if entirely needed...
	int r_returnInt = 0;
	m_changedNode = false;
	m_previousUserCmd = m_currentUserCmd;
	m_currentUserCmd = p_userCMD;

	if(!m_hasWon)
		HandleAbilities();
	//Code for playervsplayercollisionaftermath
	if (m_collisionAfterMath)
	{
		CollisionAftermath(p_dt);
	}

	if (m_state == STARTING)
		StartupSpam();
	else//if you ain't startin', you're boostin'
		Acceleration(p_dt);
	if (m_state == NORMAL || m_state == IMMORTAL)	
	{
		Rotation(p_dt);
	}
	//if(m_state!=FINISHING)
	MovementAlongLogicalMap(p_dt);
	SetDirection();

	////Fix from logical map to actual world position and orientation
	FixUpVectorRotation(m_angle);
	//now rotating around the normal

	FixOffsetFromCenterSpline();
	//now offset from the center, following the tube edge
	if(!m_currentUserCmd.rightRetardButtonPressed)
		BobOffset();
	//minor bob offset. Pisses mysterisk off
	DampDirectionRotation(p_dt);

	UpdateWorldMatrix();
	if(m_gravityShifting)
		GravityShift(m_gravityShiftProgress);
	//matrices now updated. Ready to be grabbed from the GameScreen

	UpdateCollisionBox();
	if (!m_gravityShifting&&m_state!=FINISHING&&m_state!=IMMORTAL)
		r_returnInt = WallPlacement(p_dt);
	if(m_state!= STARTING)
		UpdateTimers(p_dt);



	if (m_state == IMMORTAL)
	{
		DeathShake();
	}

	return r_returnInt;
}


//////BIG FAT ARISTOCRAT METHODS
void Player::StartupSpam()
{
	if (m_previousUserCmd.aButtonPressed && !m_currentUserCmd.aButtonPressed)
	{
		m_aButtonPressedAtStart++;
	}
}

void Player::HandleAbilities()
{
	if(m_abilityCooldown<=0)
	{
		if(m_currentUserCmd.bButtonPressed)//gravity shift
		{
			m_gravityShifting = true;
			m_abilityCooldown = m_gravityShiftCooldown;
			m_usedAbilityMaxCoolDown = m_gravityShiftCooldown;
		}
	}
}

void Player::Acceleration(float p_dt)
{
	////Acceleration
	//boost acceleration
	if (m_currentUserCmd.rightTriggerPressed && m_boostMeter > 0&& m_speed< m_maxBoostSpeed)
	{
		//check if max boost speed is attained, otherwise accelerate
		if (m_maxBoostSpeed > m_speed)
		{
			m_speed += p_dt*m_boostAcceleration;
		}
		else
		{
			//if you've reached the max boost speed, you stay there
			m_speed = m_maxBoostSpeed;
		}
		//lower remaining boost
		m_boostMeter -= p_dt;
	}

	//break
	else if(m_currentUserCmd.rightBumberPressed)
	{
		if(m_speed>m_minSpeed)
			m_speed -= m_break*p_dt+m_breakCoefficient*m_speed*p_dt;
		else 
			m_speed = m_minSpeed;

	}
	//ordinary acceleration
	else
	{
		if(m_speed>m_maxBoostSpeed)
		{
			m_speed-=p_dt*m_speed*m_megaBoostDecelerationCoefficient;
		}
		else if (m_speed < m_maxSpeed)
		{
			m_speed += p_dt*m_acceleration;
		}
		//decelerate if you're above ordinary max speed
		else
		{
			m_speed -= p_dt*m_deceleration;
		}
	}
	if(m_currentUserCmd.xButtonPressed)
		m_speed += p_dt*m_boostAcceleration*2;
	if(m_finishProgress>=1)
		m_speed=0;
}

void Player::Rotation(float p_dt)
{
	m_previousAngle = m_angle;
	m_deltaAngle = 0;
	m_deltaAngle = m_currentUserCmd.Joystick.x*m_rotateSpeed;
	m_angle += m_deltaAngle;
	if(m_deltaAngle!=0)
	{
		m_currentAngle = m_deltaAngle;
	}
}

void Player::MovementAlongLogicalMap(float p_dt)
{
	////movement along logical map
	//adds distance from the current node based on speed and time since last update
	m_distance += m_speed*p_dt;
	//Checks of distance exceeds distance to new node. In other words, if the player "overshoots" the next node
	while (m_distance >= m_mathHelper.Abs(m_mapNode->m_normal))
	{
		float t_remainingDistance = m_distance - m_mathHelper.Abs(m_mapNode->m_normal);
		m_distance = t_remainingDistance;
		m_mapNode = m_mapNode->m_nextNode;
		m_changedNode = true;
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
	t_interpolation = m_distance / m_mathHelper.Abs(m_mapNode->m_normal);
	XMFLOAT3 t_frontNormalComponent = m_mathHelper.FloatMultiVec(t_interpolation, m_mathHelper.Normalize(m_mapNode->m_nextNode->m_normal));
	XMFLOAT3 t_currentNormalComponent = m_mathHelper.FloatMultiVec(1 - t_interpolation, m_mathHelper.Normalize(m_mapNode->m_normal));
	m_direction = m_mathHelper.Normalize(m_mathHelper.VecAddVec(t_frontNormalComponent, t_currentNormalComponent));
	m_unmodifiedTarget = m_direction;


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

	UpdateWorldMatrix();
	//Matrix now updates. Ready to be grabbed from the gamescreen
}

void Player::UpdateCollisionBox()
{


	//XMFLOAT3 t_vector = t_mathHelper.CrossProduct(m_direction, m_upVector); kanske inte beh�vs

	//XMVECTOR t_eyeVector = XMLoadFloat3(&m_position);
	//XMVECTOR t_targetVector = XMLoadFloat3(&m_direction);
	//XMVECTOR t_upVector = XMLoadFloat3(&m_up);

	//XMMATRIX t_boxOrientationMatrix = XMMatrixLookToLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_direction), XMLoadFloat3(&m_up)); //matrixyo


	XMFLOAT4 t_quarternion = XMFLOAT4(0, 0, 0, 1);
	XMVECTOR t_boxOrientationVector = XMLoadFloat4(&t_quarternion);


	XMStoreFloat4(&t_quarternion, XMQuaternionRotationMatrix(XMMatrixInverse(nullptr,XMLoadFloat4x4(&m_worldMatrix))));


	//t_boxOrientationVector = XMVector4Transform(t_boxOrientationVector, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_worldMatrix)));
	//t_boxOrientationVector = XMVector4Normalize(t_boxOrientationVector);
	//XMStoreFloat4(&t_quarternion, t_boxOrientationVector);



	m_box.Center = m_position;
	m_box.Extents = m_playerShipBoxExtents; 
	m_box.Orientation = t_quarternion;



	//XMFLOAT3 t_position = XMFLOAT3 (0,0,0);
	//XMFLOAT4 t_quarternion = XMFLOAT4(0,0,0,1);
	//BoundingOrientedBox t_box = BoundingOrientedBox(t_position, m_playerShipBoxExtents, t_quarternion);
	//XMMATRIX t_worldMatrix = XMLoadFloat4x4(&m_worldMatrix);
	//t_box.Transform(t_box, t_worldMatrix);
	//m_box = t_box;


}

int Player::WallPlacement(float p_dt)
{
	if(m_wallMeter>m_maxWalls+1)
		m_wallMeter = m_maxWalls+1;
	//if the players wants to poop out walls
	if (m_currentUserCmd.leftTriggerPressed && m_coolDown <= 0 && m_wallMeter >= 1)
	{
		m_wallMeter -= 1;
		PlaceWall();
		m_coolDown = m_maxCooldown;
		return  1;
	}
	//if the players meter is full
	//yes, this could also have been one hell of a long if statement but fuggit
	else if (m_wallMeter > m_maxWalls)//m_coolDown <= 0 && 
	{
		m_wallMeter = m_maxWalls;
		m_wallMeter -= 1;

		m_coolDown = m_maxCooldown;

		PlaceWall();
		return  1;
	}
	return 0;

}

void Player::UpdateTimers(float p_dt)
{
	////immortal and death timers
	m_coolDown -= p_dt;
	m_immortalTimer -= p_dt;
	if (m_immortalTimer < 0&&m_state==IMMORTAL&&!m_gravityShifting&&!m_hasWon)
	{
		m_state = NORMAL;
		if(m_wallMeter > m_maxWalls+1)
		{
			m_wallMeter = m_maxWalls+1;
		}
		m_coolDown = m_maxCooldown;
	}
	m_deathTimer -= p_dt;
	if (m_gravityShifting)
		m_gravityShiftProgress += p_dt*m_gravityShiftSpeed;
	m_wallMeter += p_dt*m_wallGain*(4 - m_racePos);


	m_bobTimer += p_dt*m_bobFrequency;
	m_abilityCooldown -= p_dt;

	if(m_state==FINISHING &&!m_gravityShifting)
	{
		m_finishProgress += 0.3*p_dt;
		m_speed = (1-m_finishProgress)*m_finishSpeed;
		if(m_finishProgress>1)
			m_finishProgress = 1;
	}
}





/////PUNY PEASANT SLAVE METHODS




void Player::PlaceWall()
{
	m_lastPlacedWall = new PlayerWall(XMFLOAT3(0, 0, 1), &m_position, &m_wallPlacementDirection, &m_up, m_playerIndex, m_wallBoxExtents);
	m_placedWalls.push_back(m_lastPlacedWall);
	m_mapNode->AddWall(m_lastPlacedWall);
}

void Player::FixUpVectorRotation(float p_angle)
{
	MathHelper t_mathHelper = MathHelper();

	//Rotates the radius vector with angle around the normal/direction. Sets it to the up vector. Azooka made uber-hax.
	XMMATRIX t_rotationmatrix = XMMatrixRotationAxis(XMLoadFloat3(&t_mathHelper.Normalize(m_mapNode->m_normal)), p_angle);
	XMVECTOR t_upVector = XMLoadFloat3(&m_mapNode->m_radiusVector);
	t_upVector = XMVector3Transform(t_upVector, t_rotationmatrix);
	XMStoreFloat3(&m_up, t_upVector);
	m_up = t_mathHelper.Normalize(m_up);
	m_unmodifiedUp = m_up;
}

void Player::FixOffsetFromCenterSpline()
{
	m_position = m_mathHelper.VecAddVec(m_position, m_mathHelper.FloatMultiVec(-m_radius+m_vehicleHoverDistance, m_up));
}

void Player::BobOffset()
{
	float t_bob = sin(m_bobTimer*3.1415);

	if(m_state == FINISHING&&t_bob<0.01)
		m_bobFrequency = 0;

	XMVECTOR t_bobVector = XMLoadFloat3(&m_bobOffset);
	XMVECTOR t_unmodifidiedUpVector = XMLoadFloat3(&m_unmodifiedUp);
	t_bobVector+=t_bob*t_unmodifidiedUpVector*m_bobIntensity;
	XMStoreFloat3(&m_bobOffset, t_bobVector);




	////OLD BOB OFFSET STUFF. SCREW THAT
	//declaring offset variables
	//float t_bobX, t_bobY, t_bobZ;

	//uniorm_real_distribution<loat> distribution(-1, 1);
	//t_bobX = distribution(m_randomGenerator);
	//t_bobY = distribution(m_randomGenerator);
	//t_bobZ = distribution(m_randomGenerator);

	//m_bobOffset = m_mathHelper.VecAddVec(m_bobOffset, m_mathHelper.FloatMultiVec(1, XMFLOAT3(t_bobX, t_bobY, t_bobZ)));

}

void Player::DeathShake()
{
	float t_bobX, t_bobY, t_bobZ;

	uniform_real_distribution<float> distribution(-1, 1);
	t_bobX = distribution(m_randomGenerator);
	t_bobY = distribution(m_randomGenerator);
	t_bobZ = distribution(m_randomGenerator);

	m_bobOffset = m_mathHelper.VecAddVec(m_bobOffset, m_mathHelper.FloatMultiVec(pow(m_immortalTimer / m_deathShakeMaxIntensity, m_deathShakeIntensityDrop), XMFLOAT3(t_bobX, t_bobY, t_bobZ)));
}

void Player::UpdateWorldMatrix()
{

	//float t_radius = m_finishProgress*( m_mapNode->m_radius + (m_mapNode->m_radius / 4));
	float t_radius = m_finishProgress*m_cameraTrailDistanceUp;
	XMFLOAT3 t_position = XMFLOAT3(m_position.x, m_position.y, m_position.z);


	float t_progressX = sin(m_finishProgress*3.1415);
	float t_progressZ = cos(m_finishProgress*3.1415);
	float t_progressY = m_finishProgress;


	float t_cameraProgress = m_finishProgress*m_finishCameraFollowSpeed;
	if(t_cameraProgress >1)
		t_cameraProgress = 1;

	float t_cameraProgressX = sin(t_cameraProgress*3.1415);
	float t_cameraProgressZ = cos(t_cameraProgress*3.1415);
	float t_cameraProgressY = m_finishProgress;







	float t_finishRotation = m_finishProgress*m_finalDirection*m_finishAngle;

	float t_finishSlide = m_finishSpeed*m_finishProgress;

	float t_cameraTailDistanceTarget = m_cameraTrailDistanceTarget;
	if(m_currentUserCmd.leftBumberPressed)
		t_cameraTailDistanceTarget*=-1;


	XMVECTOR t_vehicleEyeVector = XMLoadFloat3(&t_position);
	XMVECTOR t_vehicleTargetVector = XMLoadFloat3(&m_direction);
	XMVECTOR t_vehicleUpVector = XMLoadFloat3(&m_up);

	////////////////////VERY DUBIOUS ABOUT THIS ONE!!!////////////
	//t_vehicleEyeVector+=XMLoadFloat3(&m_unmodifiedTarget)*t_finishSlide;

	//ordinary camera eye position offsets
	XMVECTOR t_cameraEyeVector = t_vehicleEyeVector + t_vehicleUpVector*m_cameraTrailDistanceUp + t_cameraProgressZ*t_cameraTailDistanceTarget*t_vehicleTargetVector*-1;


	XMVECTOR t_cameraUpVector = t_vehicleUpVector;
	XMVECTOR t_cameraTargetVector = t_vehicleEyeVector;

	XMVECTOR t_vehicleUnmoddedTargetVector = t_vehicleTargetVector;

	////VEHICLE TILT
	//rotate along target vector
	XMMATRIX t_directionRotationMatrixTarget = XMMatrixRotationAxis(t_vehicleTargetVector, m_currentAngle * 5);					//////////////////////////MAKE SURE YOU CHANGE THIS HARDCODED 10 CRAP////////////////
	t_vehicleUpVector = XMVector3Transform(t_vehicleUpVector, t_directionRotationMatrixTarget);
	t_vehicleUpVector = XMVector3Normalize(t_vehicleUpVector);
	XMMATRIX t_directionRotationMatrixUp;

	//rotate along new up vector
	//if(m_state !=FINISHING)
	//	t_directionRotationMatrixUp = XMMatrixRotationAxis(t_vehicleUpVector, m_currentAngle * 20 * (3.1415/(m_speed+1)));//Gives a bad value during countdown				//////////////////////////MAKE SURE YOU CHANGE THIS HARDCODED 10 CRAP////////////////
	//else
	//	t_directionRotationMatrixUp = XMMatrixRotationAxis(t_vehicleUpVector, m_currentAngle * 20 * (3.1415/(m_finishSpeed+1))+t_finishRotation);

	///////////////////////NEW STUFF//////////////
	float t_turnCoefficient = 0;
	if(m_speed > m_maxBoostSpeed)
		t_turnCoefficient = 0;
	else
		t_turnCoefficient = (m_maxBoostSpeed-m_speed)/m_maxBoostSpeed;



	if(m_state !=FINISHING)
		t_directionRotationMatrixUp = XMMatrixRotationAxis(t_vehicleUpVector, m_currentAngle*20*sin((3.1415/2)*t_turnCoefficient));//Gives a bad value during countdown				//////////////////////////MAKE SURE YOU CHANGE THIS HARDCODED 10 CRAP////////////////
	else
		t_directionRotationMatrixUp = XMMatrixRotationAxis(t_vehicleUpVector, m_currentAngle*20+t_finishRotation);
	/////////////////////NEW STUFF ENDS///////////////

	t_vehicleTargetVector = XMVector3Transform(t_vehicleTargetVector, t_directionRotationMatrixUp);
	t_vehicleTargetVector = XMVector3Normalize(t_vehicleTargetVector);

	////CAMERA MATRIX GENERATION

	//update camera angle

	//TIME FOR RETARD HAXX!! makes the camera chase after the angle
	int t_intBuffer = 1000000;
	int t_deltaAngleInt = (int)(m_deltaAngle * t_intBuffer);
	int t_cameraAngleInt = (int)(m_cameraAngle * t_intBuffer);

	if (t_deltaAngleInt > t_cameraAngleInt)
		m_cameraAngle += m_cameraFollowSpeed;
	else if (t_cameraAngleInt > t_deltaAngleInt)
		m_cameraAngle -= m_cameraFollowSpeed;


	//camera delayed follow thingy
	XMMATRIX t_cameraRotationMatrix = XMMatrixRotationAxis(t_vehicleUnmoddedTargetVector, -m_deltaAngle - m_cameraAngle * 10);
	t_cameraUpVector = XMVector3Transform(t_cameraUpVector, t_cameraRotationMatrix);
	t_cameraUpVector = XMVector3Normalize(t_cameraUpVector);



	////special finish line position offsets (can be done during race too. God bless trigonometry)
	//offsets to the right of the Vehicle
	t_cameraEyeVector+=m_cameraTrailDistanceRight*t_cameraProgressX*XMVector3Cross(XMLoadFloat3(&m_unmodifiedTarget), XMLoadFloat3(&m_unmodifiedUp));


	/////FINAL CAMERA MATRIX SET/////////
	XMStoreFloat4x4(&m_cameraMatrix, XMMatrixLookAtLH(t_cameraEyeVector+0*XMLoadFloat3(&m_unmodifiedTarget), t_cameraTargetVector+XMLoadFloat3(&m_unmodifiedUp)*t_radius*t_cameraProgressY, t_cameraUpVector));


	XMVECTOR t_bobOffsetVector = XMLoadFloat3(&m_bobOffset);
	/////FINAL VEHICLE WORLD MATRIX SET//////////
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixLookToLH(t_vehicleEyeVector + t_bobOffsetVector, t_vehicleTargetVector, t_vehicleUpVector));

	//store direction of car for wall placement
	XMStoreFloat3(&m_wallPlacementDirection, t_vehicleTargetVector);


	if(!m_gravityShifting)
	{
		XMStoreFloat3(&m_direction, t_vehicleTargetVector);
		XMStoreFloat3(&m_up, t_vehicleUpVector);
	}
	m_bobOffset = XMFLOAT3(0, 0, 0);
}

void Player::GravityShift(float p_progress)
{
	float t_cameraTailDistanceTarget = m_cameraTrailDistanceTarget;
	if(m_currentUserCmd.leftBumberPressed)
		t_cameraTailDistanceTarget*=-1;
	m_state = IMMORTAL;
	//time for reversing
	XMVECTOR t_unmodifiedEyeVector = XMLoadFloat3(&m_position);
	XMVECTOR t_unmodifiedUpVector = XMLoadFloat3(&m_up);
	XMVECTOR t_unomdifiedTargetVector = XMLoadFloat3(&m_direction);

	XMVECTOR t_eyeVector = XMLoadFloat3(&m_position);
	XMVECTOR t_upVector = XMLoadFloat3(&m_up);
	XMVECTOR t_targetVector = XMLoadFloat3(&m_direction);


	float t_targetAngle = 3.14; //pi, half a circle

	//makes the camera work faster than the car
	float t_cameraProgress = m_gravityShiftCameraMoveSpeed * p_progress;
	if (t_cameraProgress > 1)
		t_cameraProgress = 1;
	//m_radius+m_vehicleHoverDistance
	float t_radius = m_radius + cos(p_progress*3.14)*m_vehicleHoverDistance;
	float t_cameraRadius = m_radius + cos(t_cameraProgress*3.14)*m_vehicleHoverDistance;


	//XMVECTOR t_cameraEyeVector = t_eyeVector + t_upVector*m_cameraTrailDistanceUp + m_cameraTrailDistanceTarget*t_targetVector*-1;
	XMVECTOR t_cameraEyeVector = t_eyeVector;
	XMVECTOR t_cameraUpVector = t_upVector;
	XMVECTOR t_cameraTargetVector = t_eyeVector;

	//move up from the car
	t_eyeVector += t_upVector*t_radius*p_progress * 2;

	//gets the camera
	t_cameraTargetVector += t_upVector*t_cameraRadius*t_cameraProgress * 2;
	//t_cameraEyeVector += t_upVector*t_radius*t_cameraProgress * 2;

	XMMATRIX t_directionRotationMatrixTarget = XMMatrixRotationAxis(t_targetVector, p_progress*t_targetAngle);
	t_upVector = XMVector3Transform(t_upVector, t_directionRotationMatrixTarget);
	t_upVector = XMVector3Normalize(t_upVector);

	XMMATRIX t_cameraDirectionRotationMatrixTarget = XMMatrixRotationAxis(t_targetVector, t_cameraProgress*t_targetAngle);
	t_cameraUpVector = XMVector3Transform(t_cameraUpVector, t_cameraDirectionRotationMatrixTarget);
	t_cameraUpVector = XMVector3Normalize(t_cameraUpVector);

	if (p_progress > 1)
	{
		m_gravityShiftProgress = 0;
		m_gravityShifting = false;
		XMStoreFloat3(&m_position, t_eyeVector);
		XMStoreFloat3(&m_up, t_upVector);
		m_angle += 3.14;
		m_cameraAngle = 0;
		m_state = NORMAL;
	}


	t_unmodifiedUpVector = XMVector3Normalize(t_unmodifiedUpVector);


	//offsets the camera's position. THIS IS FRIGGIN PERFECT!!
	t_cameraEyeVector += t_unmodifiedUpVector*t_cameraRadius * 2 * t_cameraProgress + cos(t_cameraProgress*3.14)*t_unmodifiedUpVector*m_cameraTrailDistanceUp;

	//offsets behind
	t_cameraEyeVector += -t_cameraTailDistanceTarget*t_unomdifiedTargetVector;
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixLookToLH(t_eyeVector, t_targetVector, t_upVector));
	XMStoreFloat4x4(&m_cameraMatrix, XMMatrixLookAtLH(t_cameraEyeVector, t_cameraTargetVector, t_cameraUpVector));
}


void Player::PostGravityShiftFix()
{

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
		if (p_corners[i].x > t_maxX)
			t_maxX = p_corners[i].x;
		else if (p_corners[i].x < t_minX)
			t_minX = p_corners[i].x;

		//gets max and min y
		if (p_corners[i].y > t_maxY)
			t_maxY = p_corners[i].y;
		else if (p_corners[i].y < t_minY)
			t_minY = p_corners[i].y;

		//gets max and min Z
		if (p_corners[i].z > t_maxZ)
			t_maxZ = p_corners[i].z;
		else if (p_corners[i].z < t_minZ)
			t_minZ = p_corners[i].z;
	}

	//checks if absolute value of min values is greater than max values (really unnecessary if models are symetric, but can TA really be trusted?
	if (t_maxX < -t_minX)
		t_maxX = -t_minX;

	if (t_maxY < -t_minY)
		t_maxY = -t_minY;

	if (t_maxZ < -t_minZ)
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
	return XMLoadFloat4x4(&m_cameraMatrix);
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
	t_distance += m_distance / 100; //S� att du kan skilja p� positioner �ven om fler spelare �r i samma mapnode
	return t_distance;
}



int Player::GetRacePosition()
{
	return m_racePos;
}

XMFLOAT3 Player::GetRadiusVector()
{
	return m_mathHelper.CrossProduct(m_unmodifiedTarget, m_unmodifiedUp);
}

XMFLOAT3 Player::GetUnmodifiedTargetVector()
{
	return m_unmodifiedTarget;
}

XMFLOAT3 Player::GetUnmodifiedUpVector()
{
	return m_unmodifiedUp;
}

float Player::GetAngle()
{
	return m_angle;
}
float Player::GetHudBoosterInfo()
{
	//apparently wants 0 to be alot of boost, and 1 to be empty
	return 1 - (m_boostMeter / m_maxBoost);
}

float Player::GetHudWallInfo()
{
	//apparently wants 0 to be alot of walls, and 1 to be empty
	return 1 - (m_wallMeter/m_maxWalls );
}

bool Player::GetImmortal()
{
	return(m_state == IMMORTAL);
}

bool Player::HasFinished()
{
	return(m_state==FINISHING);
}



int Player::GetPlayerIndex()
{
	return m_playerIndex;
}

int Player::GetNrOfAPressedAtStart()
{
	return m_aButtonPressedAtStart;
}

float Player::GetSpeed()
{
	return m_speed;
}

float Player::GetDeltaAngle()
{
	return m_deltaAngle;
}

std::vector<PlayerWall*>* Player::GetPlacedWalls()
{
	return &m_placedWalls;
}

bool Player::AbilityReady()
{
	return (m_abilityCooldown<=0);
}

int Player::CurrentLap()
{
	return m_lap;
}

float Player::GetImmortalTimer()
{
	return m_immortalTimer;
}

bool Player::DoneFinishing()
{
	return m_finishProgress>=1;
}

bool Player::IsGravityShifting()
{
	return m_gravityShiftProgress>0;
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

void Player::IncreaseBoost(int p_nrOfWallsClose, float p_dt)
{
	m_boostMeter += p_nrOfWallsClose*p_dt*m_boostGain*(m_racePos-1);
	if (m_boostMeter > m_maxBoost)
	{
		m_boostMeter = m_maxBoost;
	}
}

void Player::SetFinalDirection()
{
	if (!m_hasWon)
	{
		m_direction = m_mathHelper.FloatMultiVec(-1, m_mapNode->m_previousNode->m_normal);
		m_direction = m_mathHelper.Normalize(m_direction);
		m_position = m_mapNode->m_position;
		FixWorldPosition();
		m_hasWon = true;
	}
}

void Player::AngleMoveBack()
{
	m_angle = m_previousAngle;
	//UpdateCollisionBox();
}

void Player::SetShockwaveCooldown()
{
	m_abilityCooldown = m_shockWaveCooldown;
	m_usedAbilityMaxCoolDown = m_shockWaveCooldown;
}

void Player::NextLap()
{
	m_lap++;
}

bool Player::ChangedNode()
{
	return m_changedNode;
}

void Player::Start()
{
	m_speed = (float)(m_aButtonPressedAtStart) *m_starUpSpamSpeed;
	m_state = NORMAL;
}

void Player::Finish()
{
	m_finishSlideSpeed = m_speed*0.3;
	m_finishSpeed = m_speed;
	m_cameraAngle = 0;
	if(m_deltaAngle>0)
		m_finalDirection = 1;
	else if(m_deltaAngle<0)
		m_finalDirection = -1;
	else
	{
		//random direction if going straight
		uniform_real_distribution<float> distribution(-1,1);
		float t_derpDesignator=0;
		t_derpDesignator = distribution(m_randomGenerator);
		if(t_derpDesignator>0)
			m_finalDirection = 1;
		else
			m_finalDirection = -1;
	}

	uniform_real_distribution<float> distribution(m_minFinishAngle, m_maxFinishAngle);
	m_finishAngle = distribution(m_randomGenerator);


	m_state = FINISHING;
	m_hasWon = true;
}


void Player::SetSpeed(float p_speed)
{
	m_speed += p_speed;
}

void Player::PadBoost(float p_dt)
{
	m_speed += m_boostFromPad*p_dt;
}


////scrap

void Player::Update(float p_dt, UserCMD userCMD)
{
}
void Player::UpdatePosition(float p_dt, UserCMD p_userCMD)
{
	////free moving////
	MathHelper t_mathHelper;
	m_direction.x += p_userCMD.Joystick.x;
	m_direction.y += p_userCMD.Joystick.y;
	m_direction = t_mathHelper.Normalize(m_direction);

	m_position = t_mathHelper.FloatMultiVec(p_dt, t_mathHelper.FloatMultiVec(m_speed, m_direction));
}


void Player::ChangeState(PlayerState p_state)
{
	m_state = p_state;
	switch (m_state)
	{
	case NORMAL:
		break;
	case DEAD:
		m_deathTimer = DEATH_TIME;
		break;
	case IMMORTAL:
		m_immortalTimer = IMMORTAL_TIME;
		break;
	default:
		break;
	}
}

void Player::CollisionAftermath(float p_dt)
{
	if(m_collisionAfterSpeed!=0)
	{
		m_speed -= m_collisionAfterSpeed;
		if(m_speed<0)
			m_speed =0;
		m_collisionAfterSpeed = 0;
	}




	float t_something = abs(m_collisionAngleOffset*100);

	m_collisionAfterMathTimer += p_dt;
	m_angle += (t_something - m_collisionAfterMathTimer)*m_collisionAngleOffset;
	if (m_collisionAfterMathTimer >= t_something)
	{
		m_collisionAfterMath = false;
		m_collisionAfterMathTimer = 0;
	}
}

void Player::StartCollisionAftermath(float p_sideForce, float p_targetForce, int p_sideDirection, int p_targetDirection)
{
	m_collisionAngleOffset = (p_sideForce*p_sideDirection)/m_bumpIntensity+p_sideDirection*(m_baseBumpIntensity/300);	//yes, hard-coded 300. It's madness
	m_collisionAfterSpeed = p_targetForce*p_targetDirection*m_targetBumpIntensity;
	m_collisionAfterMath = true;
}

void Player::StartShockWaveAftermath(int p_sideDirection, int p_targetDirection, float p_zValue, float p_xValue)
{
	if(m_state==NORMAL)
	{
		m_collisionAngleOffset = p_xValue * p_sideDirection * m_angleShockwavePower;
		m_collisionAfterSpeed = -1*p_zValue * p_targetDirection * m_speedShockwavePower;
		m_collisionAfterMath = true;
	}
}

void Player::DampDirectionRotation(float p_dt)

{	//TIME FOR RETARD HAXX!! makes the camera chase after the angle
	int t_intBuffer = 1000000;
	int t_deltaAngleInt = (int)(m_deltaAngle * t_intBuffer);
	int t_currentAngleInt = (int)(m_currentAngle * t_intBuffer);

	if(m_currentAngle > t_currentAngleInt)
	{
		m_currentAngle+=m_dampShipRotation*p_dt;
	}
	else if(m_currentAngle < t_currentAngleInt)
	{
		m_currentAngle-=m_dampShipRotation*p_dt;
	}

}
float Player::GetMaxImmortalTimer()
{
	return m_maxImmortalTimer;
}

bool Player::GetDrawn()
{
	return m_isDrawn;
}

void Player::SetDrawn(bool p_condition)
{
	m_isDrawn = p_condition;
}

float Player::GetRemainingAbilityCooldown()
{
	float t_tekis = m_abilityCooldown < 0 ? 0:m_abilityCooldown;
	return t_tekis/m_usedAbilityMaxCoolDown;
}