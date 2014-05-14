#pragma once
#include <vector>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <StaticObj.h>
#include <PlayerWall.h>
#include <Player.h>
#include <PhysicsManager.h>

using namespace DirectX;
class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);
	PhysicsManager* m_physMan;


	int PlayerVsObj(BoundingOrientedBox* p_player, std::vector<StaticObj*>* p_collisionBoxes);
	int PlayerVsPlayerWall(BoundingOrientedBox* p_player, std::vector<PlayerWall*>p_collisionBoxes, int p_playerIndex); //Ändra för playerwall?
	void PlayerVsPlayer(std::vector<Player*>p_playerList);
	void CollisionManager::ShockWaveCollision(std::vector<Player*> p_playerList, int p_playerWithShockwave);
	void CollisionManager::SetPlayerVsPlayer(Player* p_currentPlayer, Player* p_intersectingPlayer);
	void CollisionManager::SetShockWaveCollision(Player* p_playerWithShockWave, Player* p_intersectingPlayer);

private:
	bool IntersectionTest(BoundingOrientedBox* a, BoundingOrientedBox* b);
};

