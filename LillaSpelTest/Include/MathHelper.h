#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class MathHelper
{
public:
	MathHelper(void);
	~MathHelper(void);

	float MathHelper::DotProduct(XMFLOAT3 p_vec1, XMFLOAT3 p_vec2);
	XMFLOAT3 MathHelper::FloatMultiVec(float p_float, XMFLOAT3 p_vec);
	XMFLOAT3 MathHelper::Normalize(XMFLOAT3 p_vec);
	XMFLOAT3 MathHelper::VecSubVec(XMFLOAT3 p_vec1, XMFLOAT3 p_vec2);
	XMFLOAT3 MathHelper::VecAddVec(XMFLOAT3 p_fl31, XMFLOAT3 p_fl32);
	float MathHelper::Abs(XMFLOAT3 p_fl3);
};

