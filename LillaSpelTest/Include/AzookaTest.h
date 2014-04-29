#pragma once
#include <string>
#include <vector>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include "DebugCamera.h"
#include "UserCMD.h"

using namespace DirectX;

class AzookaTest
{
public:
	AzookaTest(void);
	~AzookaTest(void);
	XMMATRIX GetDebugCameraWorldMatrix(UserCMD* p_userCMD, float p_dt);
	DebugCamera m_debugCamera;


	void Run();
};

