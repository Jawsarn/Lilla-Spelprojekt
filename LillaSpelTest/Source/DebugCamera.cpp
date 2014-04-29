#include "DebugCamera.h"


DebugCamera::DebugCamera(void)
{
	m_position = XMFLOAT3(0,0,0);
	m_target = XMFLOAT3(0,0,1);
	m_up = XMFLOAT3(0,1,0);
}


DebugCamera::~DebugCamera(void)
{
}


XMMATRIX DebugCamera::GetWorldMatrix(UserCMD* p_userCMD, float p_dt)
{
	MathHelper t_mathHelper;
	XMMATRIX r_matrix;

	float t_moveSpeed = 20;

	////Fix rotation
	//create right-vector (local x-axis)
	XMFLOAT3 t_right= t_mathHelper.CrossProduct(m_target, m_up);

	//load to XMVECTOR
	XMVECTOR t_rightVector = XMLoadFloat3(&t_right);
	XMVECTOR t_upVector = XMLoadFloat3(&m_up);
	XMVECTOR t_targetvector = XMLoadFloat3(&m_target);

	
	float t_angleCoefficient = 1;
	float t_angleVertical = p_userCMD->rightJoystick.x*t_angleCoefficient*p_dt;
	float t_angleHorizontal = p_userCMD->rightJoystick.y*t_angleCoefficient*p_dt;

	//rotate up and target around the right-axis (local x-axis)
	XMMATRIX t_horizontalRotation = XMMatrixRotationAxis(t_rightVector, t_angleHorizontal);
	t_upVector = XMVector3Transform(t_upVector, t_horizontalRotation);
	t_upVector = XMVector3Normalize(t_upVector);
	t_targetvector = XMVector3Transform(t_targetvector, t_horizontalRotation);
	t_targetvector = XMVector3Normalize(t_targetvector);
	//rotate up around the new target (new local y-axis)
	XMMATRIX t_verticalRotation = XMMatrixRotationAxis(t_upVector, t_angleVertical);
	t_targetvector = XMVector3Transform(t_targetvector, t_verticalRotation);

	//store back to XMFLOAT3
	XMStoreFloat3(&m_target, t_targetvector);
	XMStoreFloat3(&m_up, t_upVector);


	////Fix position
	//use left stick to move in local xz-plane
	XMVECTOR t_positionVector = XMLoadFloat3(&m_position);
	float t_moveX = -p_userCMD->Joystick.x*t_moveSpeed*p_dt;
	float t_moveZ = p_userCMD->Joystick.y*t_moveSpeed*p_dt;
	
	XMVECTOR t_moveXVector = t_rightVector*t_moveX;
	XMVECTOR t_moveZVector = t_targetvector*t_moveZ;

	XMVECTOR t_moveYVector = XMLoadFloat3(&XMFLOAT3(0,0,0));
	//move down along local y-axis
	if(p_userCMD->leftBumberPressed)
	{
		t_moveYVector = t_upVector*t_moveSpeed*p_dt;
	}
	//move up along local y-axis
	if(p_userCMD->rightBumberPressed)
	{
		t_moveYVector = -t_upVector*t_moveSpeed*p_dt;
	}


	t_positionVector += t_moveXVector+t_moveZVector+t_moveYVector;

	//t_positionVector += t_targetvector*t_moveSpeed*p_dt;
	XMStoreFloat3(&m_position, t_positionVector);

	r_matrix = XMMatrixLookToLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));
	return r_matrix;
}