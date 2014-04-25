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

XMFLOAT3 MathHelper::Normalize(XMFLOAT3 p_fl3)
{
	XMVECTOR t_vec= XMLoadFloat3(&p_fl3);
	XMFLOAT3 r_fl3;
	XMStoreFloat3(&r_fl3, (XMVector3Normalize(t_vec)));
	return r_fl3;
}

XMFLOAT3 MathHelper::VecSubVec(XMFLOAT3 p_fl31, XMFLOAT3 p_fl32) //första minus den andra
{
	XMVECTOR t_vec1 = XMLoadFloat3(&p_fl31);
	XMVECTOR t_vec2 = XMLoadFloat3(&p_fl32);
	XMFLOAT3 r_vec;
	XMStoreFloat3(&r_vec, t_vec1-t_vec2);
	return r_vec;
}

XMFLOAT3 MathHelper::VecAddVec(XMFLOAT3 p_fl31, XMFLOAT3 p_fl32) //första minus den andra
{
	XMVECTOR t_vec1 = XMLoadFloat3(&p_fl31);
	XMVECTOR t_vec2 = XMLoadFloat3(&p_fl32);
	XMFLOAT3 r_vec;
	XMStoreFloat3(&r_vec, t_vec1+t_vec2);
	return r_vec;
}


float MathHelper::Abs(XMFLOAT3 p_fl3) //vectorlängd
{
	XMVECTOR t_vec = XMLoadFloat3(&p_fl3);
	XMFLOAT3 r_vec;
	XMStoreFloat3(&r_vec, XMVector3Length(t_vec));
	return r_vec.x; //vetinteriktigtomdetfunkar varför vill den att jag sparar ner den som en vector? :(
}

XMFLOAT3 MathHelper::CrossProduct(XMFLOAT3 p_fl31, XMFLOAT3 p_fl32)
{
	XMVECTOR t_vec1 = XMLoadFloat3(&p_fl31);
	XMVECTOR t_vec2 = XMLoadFloat3(&p_fl32);
	XMFLOAT3 r_fl3;
	XMStoreFloat3(&r_fl3, XMVector3Cross(t_vec1, t_vec2));
	return r_fl3;
}