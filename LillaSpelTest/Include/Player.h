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

enum PlayerState{NORMAL,DEAD,IMMORTAL, STARTING, FINISHING};

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
	float m_megaBoostDecelerationCoefficient;
	float m_break;
	float m_breakCoefficient;
	float m_minSpeed;
	float m_boostFromPad;
	float m_starUpSpamSpeed;

	//rotation stuff
	float m_rotateSpeed;
	float m_angle;
	float m_deltaAngle;
	float m_collisionAngleOffset;
	bool m_collisionAfterMath;
	float m_collisionAfterSpeed;
	float m_targetBumpIntensity;

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

	float m_bumpIntensity;
	float m_baseBumpIntensity;
	float m_angleShockwavePower;
	float m_speedShockwavePower;
	XMFLOAT3 m_unmodifiedTarget;
	XMFLOAT3 m_unmodifiedUp;

	float m_cameraAngle;
	float m_cameraFollowSpeed;

	float m_cameraTrailDistanceUp;
	float m_cameraTrailDistanceTarget;
	float m_cameraTrailDistanceRight;

	float m_deathShakeMaxIntensity;
	float m_deathShakeIntensityDrop;

	bool m_hasWon;
	int m_lap;

	float m_gravityShiftProgress;
	bool m_gravityShifting;
	float m_gravityShiftCameraMoveSpeed;
	float m_gravityShiftSpeed;

	float m_bobTimer;
	float m_bobFrequency;
	float m_bobIntensity;


	float m_previousAngle;


	float m_abilityCooldown;
	float m_shockWaveCooldown;
	float m_gravityShiftCooldown;

	float m_finishProgress;
	float m_finishSpeed;
	float m_finishAngle;
	int m_finalDirection;
	float m_finishCameraFollowSpeed;
	float m_minFinishAngle;
	float m_maxFinishAngle;
	float m_finishSlideSpeed;
	float m_finishSlideSpeedCoefficient;

	float m_dampShipRotation;
	float m_currentAngle;
	bool m_changedNode;
	bool m_isDrawn;


	float m_radius;
	float m_vehicleHoverDistance;
	float m_usedAbilityMaxCoolDown;

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
	float GetAngle();
	float GetMaxImmortalTimer();
	bool GetDrawn();
	void SetDrawn(bool p_condition);
	float GetRemainingAbilityCooldown();

	int GetRacePosition();
	XMFLOAT3 GetRadiusVector();
	XMFLOAT3 GetUnmodifiedUpVector();
	XMFLOAT3 GetUnmodifiedTargetVector();
	float GetHudBoosterInfo();
	float GetHudWallInfo();
	bool GetImmortal();
	bool HasFinished();
	int GetPlayerIndex();
	int GetNrOfAPressedAtStart();
	float GetSpeed();
	float GetDeltaAngle();
	std::vector<PlayerWall*>* GetPlacedWalls();
	bool AbilityReady();
	int CurrentLap();
	float GetImmortalTimer();
	bool DoneFinishing();


	//modifiers
	void Die();
	void Start();
	void Finish();
	void SetSpeed(float p_speed);
	void PadBoost(float p_dt);
	void StartCollisionAftermath(float p_sideForce, float p_targetForce, int p_sideDirection, int p_targetDirection);
	void StartShockWaveAftermath(int p_sideDirection, int p_targetDirection, float p_zValue, float p_xValue);
	void SetPlayerBoost(float p_boost);
	void SetPlayerRacePosition(int p_pos);
	void IncreaseBoost(int p_nrOfWallsClose, float p_dt);
	void SetFinalDirection();
	void AngleMoveBack();
	void SetShockwaveCooldown();
	void NextLap();
	bool ChangedNode();

private:

	void CleanUp();

	//Update method
	void StartupSpam();
	void HandleAbilities();
	void Acceleration(float p_dt);
	void Rotation(float p_dt);
	void MovementAlongLogicalMap(float p_dt);
	void SetDirection();
	void FixWorldPosition();
	void DampDirectionRotation(float dt);

	void UpdateCollisionBox();
	int WallPlacement(float p_dt);
	void UpdateTimers(float p_dt);

	//Help methods
	void FixUpVectorRotation(float p_angle);
	void FixOffsetFromCenterSpline();

	void StartCoolDown(float p_whatAbility);
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

