#include "Camera.h"




Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

XMVECTOR Camera::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 Camera::GetPosition()const
{
	return mPosition;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR Eye = XMLoadFloat3(&pos);
	XMVECTOR At = XMLoadFloat3(&target);
	XMVECTOR Up = XMLoadFloat3(&up);
	XMMATRIX V = XMMatrixLookAtLH( Eye, At, Up );
	
	mPosition = pos;

	XMVECTOR r = XMVector3Cross(Up,At); 
	XMStoreFloat3(&mRight,r); // view space x-axis
	mUp = up; // view space y-axis
	mLook = target; // view space z-axis

	XMStoreFloat4x4(&mView,V);
}

void Camera::SetLens(float fovY,float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;

	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f*mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f*mFovY);

	

	//mProj = XMMatrixPerspectiveFovLH(mFovY,mAspect,mNearZ,mFarZ);

	//if we had tha matrixes in 4x4 float instead
	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY,mAspect,mNearZ,mFarZ);
	XMStoreFloat4x4(&mProj,P);

}

float Camera::GetFieldOfViewX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

float Camera::GetFarDistance()
{
	return mFarZ;
}

float Camera::GetNearDistance()
{
	return mNearZ;
}
void Camera::Walk(float d)
{
	//prettey damn good
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	if(d>25)
	{
		int b =3;
	}
	
	
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	/*float y = mTerrain->GetHeight(mPosition.x,mPosition.z);

	mPosition = XMFLOAT3(mPosition.x, y, mPosition.z);*/

}

void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

	/*float y = mTerrain->GetHeight(mPosition.x,mPosition.z);

	mPosition = XMFLOAT3(mPosition.x, y, mPosition.z);*/
}
void Camera::Pitch(float angle)
{
	//Rotate up and look vector about the right vector.
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}
void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal( XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Camera::HoverY(float amount)
{
	mPosition.y += amount;
}

void Camera:: UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	//
	// Orthonormalize the right, up and look vectors.
	//
	// Make look vector unit length.
	L = XMVector3Normalize(L);

	// Compute a new corrected "up" vector and normalize it.
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// Compute a new corrected "right" vector.
	R = XMVector3Cross(U, L);

	//
	// Fill in the view matrix entries.
	//
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0,0) = mRight.x;
	mView(1,0) = mRight.y;
	mView(2,0) = mRight.z;
	mView(3,0) = x;
	mView(0,1) = mUp.x;
	mView(1,1) = mUp.y;
	mView(2,1) = mUp.z;
	mView(3,1) = y;
	mView(0,2) = mLook.x;
	mView(1,2) = mLook.y;
	mView(2,2) = mLook.z;
	mView(3,2) = z;
	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}

XMMATRIX Camera::View()const
{
	return XMLoadFloat4x4(&mView);
}
XMMATRIX Camera::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}
XMMATRIX Camera::ViewProj()const
{
	XMMATRIX m = XMMatrixMultiply(View(),Proj());
	return m;
}

void Camera::ExtractFrustumPlanes( CXMMATRIX M, std::vector<XMFLOAT4> &planes)
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