#include "PlayerWall.h"
#include "MathHelper.h"

PlayerWall::PlayerWall(void)
{

}

PlayerWall::PlayerWall(XMFLOAT3 p_color, XMFLOAT3* p_wallPos, XMFLOAT3* p_wallDir, XMFLOAT3* p_wallUp)
{
	XMVECTOR t_eye = XMLoadFloat3(p_wallPos);
	XMVECTOR t_target = XMLoadFloat3(p_wallDir);
	XMVECTOR t_up = XMLoadFloat3(p_wallUp);
	
	
	XMMATRIX t_matrix = XMMatrixLookToLH(t_eye, t_target, t_up);
	m_worldMatrix = t_matrix;
	
	MathHelper t_mathHelper = MathHelper();
	m_color = p_color;

	XMFLOAT3 t_boxExtents = XMFLOAT3(1,1,1);		////////////HARD CODED JUNK


	XMFLOAT4 t_quarternion = XMFLOAT4(0,0,0,1);
	XMVECTOR t_orientationVector = XMLoadFloat4(&t_quarternion);
	t_orientationVector = XMVector4Transform(t_orientationVector, t_matrix);
	t_orientationVector = XMVector4Normalize(t_orientationVector);
	XMStoreFloat4(&t_quarternion, t_orientationVector);
	
	
	m_box = BoundingOrientedBox(*p_wallPos, t_boxExtents, t_quarternion);
	


}


PlayerWall::~PlayerWall(void)
{
}
BoundingOrientedBox* PlayerWall::GetBox()
{
	return &m_box;
}

unsigned int PlayerWall::GetWallIndex()
{
	return m_wallIndex;
}

XMMATRIX PlayerWall::GetWorldMatrix()
{
	
	return m_worldMatrix;
}