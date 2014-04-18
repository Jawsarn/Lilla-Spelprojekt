#pragma once
//#define _XM_NO_INTRINSICS_ //fuck this include, need it to extract frustrum planes buuuuuuut it will crash other functions
#include <DirectXMath.h>
#include <vector>

//http://msdn.microsoft.com/en-gb/library/windows/desktop/ee418725(v=vs.85).aspx
using namespace DirectX;

class Camera
{
public:
	Camera(void);
	~Camera(void);
	

	// Get/Set world camera position.
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;

	float GetFieldOfViewX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);
	void HoverY(float amount);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);
	

	// After modifying camera position/orientation, call
	// to rebuild the view matrix once per frame.
	void UpdateViewMatrix();

	float GetFarDistance();
	float GetNearDistance();

	void Camera::ExtractFrustumPlanes( CXMMATRIX M, std::vector<XMFLOAT4> &planes); //combine those two
	
private:
	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 mPosition; // view space origin
	XMFLOAT3 mRight; // view space x-axis
	XMFLOAT3 mUp; // view space y-axis
	XMFLOAT3 mLook; // view space z-axis
	
	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// Cache View/Proj matrices.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};

