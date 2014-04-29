#pragma once
#include <DirectXMath.h>
#include "UserCMD.h"
#include "MathHelper.h"
using namespace DirectX;
class DebugCamera
{
public:
	DebugCamera(void);
	XMMATRIX GetWorldMatrix(UserCMD* p_userCMD, float p_dt);
	~DebugCamera(void);


private:
	XMFLOAT3 m_up;
	XMFLOAT3 m_target;
	XMFLOAT3 m_position;
};

