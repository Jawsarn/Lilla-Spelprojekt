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
	void SetPosition(XMFLOAT3 p_Position);

	float GetFieldOfViewX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	void SetView(CXMMATRIX p_Matrix);
	// Set frustum.
	void SetLens(float p_FovY, float p_Aspect, float p_Znear, float p_Zfar);

	// Define camera space via LookAt parameters.
	void LookAt(const XMFLOAT3& p_Pos, const XMFLOAT3& p_Target, const XMFLOAT3& p_Up);

	// set view matrix


	// Get View/Proj matrices.
	
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float p_Distance);
	void Walk(float p_Distance);
	void HoverY(float p_Amount);

	// Rotate the camera.
	void Pitch(float p_Angle);
	void RotateY(float p_Angle);
	

	// After modifying camera position/orientation, call
	// to rebuild the view matrix once per frame.
	void UpdateViewMatrix();

	float GetFarDistance();
	float GetNearDistance();

	void Camera::ExtractFrustumPlanes( CXMMATRIX p_Matrix, std::vector<XMFLOAT4> &o_Planes); //combine those two
	
private:
	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 m_Position; // view space origin
	XMFLOAT3 m_Right; // view space x-axis
	XMFLOAT3 m_Up; // view space y-axis
	XMFLOAT3 m_Look; // view space z-axis
	
	// Cache frustum properties.
	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;

	// Cache View/Proj matrices.
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
};

