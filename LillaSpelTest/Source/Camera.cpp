#include "Camera.h"




Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

XMVECTOR Camera::GetPositionXM()const
{
	return XMLoadFloat3(&m_Position);
}

XMFLOAT3 Camera::GetPosition()const
{
	return m_Position;
}

void Camera::LookAt(const XMFLOAT3& p_Pos, const XMFLOAT3& p_Target, const XMFLOAT3& p_Up)
{
	XMVECTOR t_Eye = XMLoadFloat3(&p_Pos);
	XMVECTOR t_At = XMLoadFloat3(&p_Target);
	XMVECTOR t_Up = XMLoadFloat3(&p_Up);
	XMMATRIX t_View = XMMatrixLookAtLH( t_Eye, t_At, t_Up );
	
	m_Position = p_Pos;

	XMVECTOR t_Right = XMVector3Cross(t_Up,t_At); 
	XMStoreFloat3(&m_Right,t_Right); // view space x-axis
	m_Up = p_Up; // view space y-axis
	m_Look = p_Target; // view space z-axis

	XMStoreFloat4x4(&m_View,t_View);
}

void Camera::SetLens(float p_FovY,float p_Aspect, float p_Znear, float p_Zfar)
{
	m_FovY = p_FovY;
	m_Aspect = p_Aspect;

	m_NearZ = p_Znear;
	m_FarZ = p_Zfar;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f*m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f*m_FovY);

	

	//mProj = XMMatrixPerspectiveFovLH(mFovY,mAspect,mNearZ,mFarZ);

	//if we had tha matrixes in 4x4 float instead
	XMMATRIX t_Projection = XMMatrixPerspectiveFovLH(m_FovY,m_Aspect,m_NearZ,m_FarZ);
	XMStoreFloat4x4(&m_Proj,t_Projection);

}
void Camera::SetView(CXMMATRIX p_Matrix)
{
	 XMStoreFloat4x4(&m_View,p_Matrix);
}
float Camera::GetFieldOfViewX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / m_NearZ);
}

float Camera::GetNearWindowWidth()const
{
	return m_Aspect * m_NearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return m_NearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return m_Aspect * m_FarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return m_FarWindowHeight;
}

float Camera::GetFarDistance()
{
	return m_FarZ;
}

float Camera::GetNearDistance()
{
	return m_NearZ;
}
void Camera::Walk(float p_Distance)
{
	//prettey damn good
	XMVECTOR t_Singed = XMVectorReplicate(p_Distance);
	XMVECTOR t_Look = XMLoadFloat3(&m_Look);
	XMVECTOR t_Pos = XMLoadFloat3(&m_Position);
	
	
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(t_Singed, t_Look, t_Pos));
}

void Camera::Strafe(float p_Distance)
{
	XMVECTOR t_Singed = XMVectorReplicate(p_Distance);
	XMVECTOR t_Right = XMLoadFloat3(&m_Right);
	XMVECTOR t_Pos = XMLoadFloat3(&m_Position);

	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(t_Singed, t_Right, t_Pos));
}
void Camera::Pitch(float p_Angle)
{
	//Rotate up and look vector about the right vector.
	XMMATRIX t_Right = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), p_Angle);

	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), t_Right));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), t_Right));
}
void Camera::RotateY(float p_Angle)
{
	// Rotate the basis vectors about the world y-axis.
	XMMATRIX t_Rotation = XMMatrixRotationY(p_Angle);

	XMStoreFloat3(&m_Right, XMVector3TransformNormal( XMLoadFloat3(&m_Right), t_Rotation));
	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), t_Rotation));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), t_Rotation));
}

void Camera::HoverY(float p_Amount)
{
	m_Position.y += p_Amount;
}

void Camera:: UpdateViewMatrix()
{
	XMVECTOR t_Right = XMLoadFloat3(&m_Right);
	XMVECTOR t_Up = XMLoadFloat3(&m_Up);
	XMVECTOR t_Look = XMLoadFloat3(&m_Look);
	XMVECTOR t_Pos = XMLoadFloat3(&m_Position);

	//
	// Orthonormalize the right, up and look vectors.
	//
	// Make look vector unit length.
	t_Look = XMVector3Normalize(t_Look);

	// Compute a new corrected "up" vector and normalize it.
	t_Up = XMVector3Normalize(XMVector3Cross(t_Look, t_Right));

	// Compute a new corrected "right" vector.
	t_Right = XMVector3Cross(t_Up, t_Look);

	//
	// Fill in the view matrix entries.
	//
	float t_X = -XMVectorGetX(XMVector3Dot(t_Pos, t_Right));
	float t_Y = -XMVectorGetX(XMVector3Dot(t_Pos, t_Up));
	float t_Z = -XMVectorGetX(XMVector3Dot(t_Pos, t_Look));

	XMStoreFloat3(&m_Right, t_Right);
	XMStoreFloat3(&m_Up, t_Up);
	XMStoreFloat3(&m_Look, t_Look);

	m_View(0,0) = m_Right.x;
	m_View(1,0) = m_Right.y;
	m_View(2,0) = m_Right.z;
	m_View(3,0) = t_X;
	m_View(0,1) = m_Up.x;
	m_View(1,1) = m_Up.y;
	m_View(2,1) = m_Up.z;
	m_View(3,1) = t_Y;
	m_View(0,2) = m_Look.x;
	m_View(1,2) = m_Look.y;
	m_View(2,2) = m_Look.z;
	m_View(3,2) = t_Z;
	m_View(0,3) = 0.0f;
	m_View(1,3) = 0.0f;
	m_View(2,3) = 0.0f;
	m_View(3,3) = 1.0f;
}

XMMATRIX Camera::View()const
{
	return XMLoadFloat4x4(&m_View);
}
XMMATRIX Camera::Proj()const
{
	return XMLoadFloat4x4(&m_Proj);
}
XMMATRIX Camera::ViewProj()const
{
	XMMATRIX m = XMMatrixMultiply(View(),Proj());
	return m;
}

void Camera::ExtractFrustumPlanes( CXMMATRIX p_Matrix, std::vector<XMFLOAT4> &o_Planes)
{
	////
	//// Left
	////
	//planes[0].x = M(0,3) + M(0,0);
	//planes[0].y = M(1,3) + M(1,0);
	//planes[0].z = M(2,3) + M(2,0);
	//planes[0].w = M(3,3) + M(3,0);
	////
	//// Right
	////
	//planes[1].x = M(0,3) - M(0,0);
	//planes[1].y = M(1,3) - M(1,0);
	//planes[1].z = M(2,3) - M(2,0);
	//planes[1].w = M(3,3) - M(3,0);
	////
	//// Bottom
	////
	//planes[2].x = M(0,3) + M(0,1);
	//planes[2].y = M(1,3) + M(1,1);
	//planes[2].z = M(2,3) + M(2,1);
	//planes[2].w = M(3,3) + M(3,1);
	////
	//// Top
	////
	//planes[3].x = M(0,3) - M(0,1);
	//planes[3].y = M(1,3) - M(1,1);
	//planes[3].z = M(2,3) - M(2,1);
	//planes[3].w = M(3,3) - M(3,1);
	////
	//// Near
	////
	//planes[4].x = M(0,2);
	//planes[4].y = M(1,2);
	//planes[4].z = M(2,2);
	//planes[4].w = M(3,2);
	////
	//// Far
	////
	//planes[5].x = M(0,3) - M(0,2);
	//planes[5].y = M(1,3) - M(1,2);
	//planes[5].z = M(2,3) - M(2,2);
	//planes[5].w = M(3,3) - M(3,2);
	//// Normalize the plane equations.
	//for(int i = 0; i < 6; ++i)
	//{
	//	XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&planes[i]));
	//	XMStoreFloat4(&planes[i], v);
	//}

}