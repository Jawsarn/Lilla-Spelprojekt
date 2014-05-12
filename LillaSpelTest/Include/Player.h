#pragma once
#include "object.h"
#include "PlayerWall.h"
#include "Struct_MapNode.h"
#include "PlayerWall.h"
#include "UserCMD.h"
#include <DirectXCollision.h>
#include "MathHelper.h"
#include <random>


using namespace DirectX;

enum PlayerState{NORMAL,DEAD,IMMORTAL, STARTING};

#define DEATH_TIME 2
#define IMMORTAL_TIME 2

class Player :
	public Object
{
	///////////////VARIABLES///////////
private:

	//player stats
	float m_distance;
	int m_racePos;
	int m_playerIndex;
	int m_aButtonPressedAtStart;

	//speed stuff
	float m_speed;
	float m_maxSpeed;
	float m_maxBoostSpeed;
	float m_acceleration;
	float m_boostAcceleration;
	float m_deceleration;

	//rotation stuff
	float m_rotateSpeed;
	float m_angle;
	float m_deltaAngle;
	float m_collisionAngleOffset;
	bool m_collisionAfterMath;

	//boost stuff
	float m_boostDecay;
	float m_boostGain;
	float m_maxBoost;
	float m_boostMeter;

	//wall stuff
	float m_wallMeter;
	float m_maxWalls;
	float m_wallGain;
	float m_coolDown;
	float m_maxCooldown;

	//Powerup stuff
	bool m_Shockwave;

	///Timers///
	float m_deathTimer;
	float m_maxDeathTimer;
	float m_immortalTimer;
	float m_maxImmortalTimer;
	float m_collisionAfterMathTimer;

	//important for actual world position stuff
	XMFLOAT3 m_logicalPosition;
	XMFLOAT3 m_up;
	XMFLOAT4X4 m_cameraMatrix;
	XMFLOAT4X4 m_worldMatrix;

	//bounding box creation stuff
	XMFLOAT3 m_wallBoxExtents;
	XMFLOAT3 m_playerShipBoxExtents;
	XMFLOAT3 m_wallPlacementDirection;

	//other stuff
	std::vector<PlayerWall*> m_placedWalls;
	MapNode* m_mapNode;
	PlayerWall* m_lastPlacedWall;

	UserCMD m_previousUserCmd;
	UserCMD m_currentUserCmd;

	MathHelper m_mathHelper;

	XMFLOAT3 m_bobOffset;
	default_random_engine m_randomGenerator;


	XMFLOAT3 m_unmodifiedTarget;
	XMFLOAT3 m_unmodifiedUp;

	float m_cameraAngle;
	float m_cameraFollowSpeed;
	float m_cameraTrailDistanceUp;
	float m_cameraTrailDistanceTarget;

	float m_deathShakeMaxIntensity;
	float m_deathShakeIntensityDrop;

	bool m_hasWon;

	float m_gravityShiftProgress;
	bool m_gravityShifting;
	float m_gravityShiftCameraMoveSpeed;

	//unused but perhaps needed stuff
	XMFLOAT3 m_color;
	PlayerState m_state;



	///////////////FUNCTIONS/////////////
public:
	Player();
	Player(MapNode* p_startNode, float p_startAngle, int p_playerIndex);
	~Player();

	void Update(float p_dt,UserCMD p_userCMD); //Updates position and dropps walls
	void UpdatePosition(float p_dt, UserCMD p_userCMD);
	int ProperUpdatePosition(float p_dt, UserCMD p_userCMD);
	std::vector<BoundingOrientedBox*> GetWallsToCheck();
	void ChangeState(PlayerState p_state);


	//Gets yo
	XMMATRIX GetCamMatrix();
	XMFLOAT3 GetPos();
	XMFLOAT3 GetDirection();
	XMMATRIX GetWorldMatrix();		//game screen grabs and sends off to graphichandle
	MapNode* GetCurrentMapNode();
	BoundingOrientedBox* GetCollisionBox();
	PlayerWall* GetLastPlacedWall();
	float GetDistanceTraveled(); 
	float GetPlayerBoost();

	int GetRacePosition();
	XMFLOAT3 GetRadiusVector();
	float GetHudBoosterInfo();
	float GetHudWallInfo();
	bool GetImmortal();
	int GetPlayerIndex();
	int GetNrOfAPressedAtStart();

	//modifiers
	void Die();
	void Start();
	void SetSpeed(float p_speed);
	void StartCollisionAftermath(float p_angle);
	void SetPlayerBoost(float p_boost);
	void SetPlayerRacePosition(int p_pos);
	void IncreaseBoost(int p_nrOfWallsClose, float p_dt);
	void SetFinalDirection();

private:


	//Update methods
	void StartupSpam();
	void Acceleration(float p_dt);
	void Rotation(float p_dt);
	void MovementAlongLogicalMap(float p_dt);
	void SetDirection();
	void FixWorldPosition();
	
	void UpdateCollisionBox();
	int WallPlacement(float p_dt);
	void UpdateTimers(float p_dt);

	//Help methods
	void FixUpVectorRotation(float p_angle);
	void FixOffsetFromCenterSpline();
	
	void PlaceWall();
	void CollisionAftermath(float p_dt);
	void UpdateWorldMatrix();
	void UpdateWallMeter(float p_dt);
	void BobOffset();
	void DeathShake();

	void GravityShift(float p_progress);
	void PostGravityShiftFix();


	//this really does deserve its own class. Or something
	XMFLOAT3 SetBoxExtents(vector<XMFLOAT3> p_corners);

};

