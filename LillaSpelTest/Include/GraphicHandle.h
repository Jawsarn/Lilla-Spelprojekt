#pragma once

#include <vector>
#include <d3d11_1.h>
#include <DirectXMath.h>

class GraphicEngine;

using namespace DirectX;

class GraphicHandle
{
private:

	GraphicHandle();
	~GraphicHandle();
	struct LightStruct 
	{
	float m_Radius;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Color;
	UINT m_LightID;
	};
	UINT m_Player[4];
	UINT m_CurrentLevel;
	UINT m_SelectionShips[4];

	std::vector <std::vector<UINT>> m_MeshLevels;
	std::vector<std::vector<UINT>> m_MeshShips;
	GraphicEngine* m_GraphicEngine;
	
	UINT m_CameraID[4];

	XMMATRIX t_Mats;
	std::vector<XMFLOAT4X4> m_SelectionShipMatrix;
	static GraphicHandle* m_Singleton;
	//void CreateShip(int p_PlayerIndex, XMFLOAT3 p_Color, CXMMATRIX p_PlayerWorld,int p_ShipIndex);
	void CreateLight(int p_PlayerIndex,XMFLOAT3 p_Color,UINT p_ObjectId, LightStruct &p_LightStruct);
public:


	static GraphicHandle* GetInstance();
	
	
	void Initialize(UINT p_Width, UINT p_Height, HWND p_Handle);
	void DrawGame();
	void ChangeHUDObject(int p_hudID, int p_hudObjID, int change());
	void UpdateDynamicLight(UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	void UpdatePlayer(int p_playerID, CXMMATRIX p_matrix);
	void UpdateCamera(UINT p_CameraID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY); //for change if we are to use quaternions, else just need to add the "jaw"s =D=D
	void UpdateSelectVehicle(float p_DeltaTime);
	void SetCamera(UINT p_CameraID, XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up);
	void CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID); //inte klar alls
	void StartGame(int p_WhatLevel,
		std::vector<int> p_WhatVehicle,
		std::vector<XMMATRIX> p_PlayerWorld,
		std::vector<XMFLOAT3>p_PlayerColor,
		CXMMATRIX p_LevelWorld,
		XMFLOAT3 p_LevelColor);
	void SelectVehicle();
	void UpdateCameraVehicleSelection(UINT p_CameraLogicID,float p_LookingAtWhatVehicle);
	void JohnSetCamera(CXMMATRIX p_World, UINT p_CameraLogicID);
	int GetAmountOfVehicles();
	//void TurnCameraSelection(float p_DeltaTime,CXMMATRIX p_Start, CXMMATRIX p_End)
};