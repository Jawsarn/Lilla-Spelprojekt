#include "PlayerWall.h"
#include "MathHelper.h"

PlayerWall::PlayerWall(void)
{

}

PlayerWall::PlayerWall(XMFLOAT3 p_color, XMFLOAT3* p_wallPos, XMFLOAT3* p_wallDir, XMFLOAT3* p_wallUp, int p_playerIndex, XMFLOAT3 p_extents)
{
	MathHelper t_mathHelper = MathHelper();
	m_color = p_color;
	m_playerIndex = p_playerIndex;


	XMVECTOR t_eye = XMLoadFloat3(p_wallPos);
	XMVECTOR t_target = XMLoadFloat3(p_wallDir);
	XMVECTOR t_up = XMLoadFloat3(p_wallUp);
	
	XMMATRIX t_matrix = XMMatrixInverse(nullptr,XMMatrixLookToLH(t_eye,t_target , t_up));



	XMStoreFloat4x4( &m_worldMatrix , t_matrix);
	XMFLOAT4 t_quarternion = XMFLOAT4(0,0,0,1);
	
	//XMStoreFloat4(&t_quarternion, XMQuaternionRotationMatrix(XMMatrixInverse(nullptr,XMLoadFloat4x4(&m_worldMatrix))));
	XMStoreFloat4(&t_quarternion, XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_worldMatrix)));


	//XMVECTOR t_orientationVector = XMLoadFloat4(&t_quarternion);
	//t_orientationVector = XMVector4Transform(t_orientationVector, t_matrix);
	//t_orientationVector = XMVector4Normalize(t_orientationVector);
	//XMStoreFloat4(&t_quarternion, t_orientationVector);

//	p_extents.y = p_extents.y -0.17; needs some fixing, made the problem a bit less noticable



	//XMFLOAT3 t_position = XMFLOAT3(0,0,0);

	m_box = BoundingOrientedBox(*p_wallPos, p_extents, t_quarternion);
	//m_box.Transform(m_box, t_matrix);
	m_sphere = BoundingSphere(*p_wallPos, p_extents.z*2);


}


PlayerWall::~PlayerWall(void)
{
}
BoundingOrientedBox* PlayerWall::GetBox()
{
	return &m_box;
}

BoundingSphere* PlayerWall::GetSphere()
{
	return &m_sphere;
}

unsigned int PlayerWall::GetWallIndex()
{
	return m_wallIndex;
}

XMMATRIX PlayerWall::GetWorldMatrix()
{
	
	return XMLoadFloat4x4( &m_worldMatrix);
}

int PlayerWall::GetPlayerIndex()
{
	return m_playerIndex;
}