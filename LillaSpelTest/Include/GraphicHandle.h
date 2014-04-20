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

	UINT m_Player[4];
	
	std::vector<std::vector<UINT>> m_Ships;
	GraphicEngine* m_GraphicEngine;

	UINT m_CameraID[4];

	static GraphicHandle* m_Singleton;
public:

	static GraphicHandle* GetInstance();
	
	
	void Initialize(UINT p_Width, UINT p_Height, HWND p_Handle);
	void DrawGame();
	void ChangeHUDObject(int p_hudID, int p_hudObjID, int change());
	void UpdateDynamicLight(UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	void UpdatePlayer(int p_playerID, CXMMATRIX p_matrix);
	void UpdateCamera(UINT p_CameraID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY); //for change if we are to use quaternions, else just need to add the "jaw"s =D=D
	void CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID); //inte klar alls


};