#include "MathHelper.h"


MathHelper::MathHelper(void)
{
}


MathHelper::~MathHelper(void)
{
}

float MathHelper::DotProduct(XMFLOAT3 p_vec1, XMFLOAT3 p_vec2)
{
	float r_skalar;
	XMStoreFloat(&r_skalar, XMVector3Dot(XMLoadFloat3(&p_vec1), XMLoadFloat3(&p_vec2)));
	return r_skalar;
}
XMFLOAT3 MathHelper::FloatMultiVec(float p_float, XMFLOAT3 p_vec)
{
	XMFLOAT3 r_vec;

	XMVECTOR t_vec = XMLoadFloat3(&p_vec);
	t_vec *= p_float;

	XMStoreFloat3(&r_vec,t_vec);
	return r_vec;

}

XMFLOAT3 MathHelper::Normalize(XMFLOAT3 p_vec)
{
	XMFLOAT3 t_vec;
	XMVector3Normalize(XMLoadFloat3(&t_vec)); //INTEFÄRDIGT
	return t_vec;
}

XMFLOAT3 MathHelper::VecSubVec(XMFLOAT3 p_vec1, XMFLOAT3 p_vec2)
{
	XMFLOAT3 temp;
	return temp;
}
