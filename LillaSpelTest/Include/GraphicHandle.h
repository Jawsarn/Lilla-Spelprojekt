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

	
	std::vector<UINT> m_Player;
	//std::vector<UINT> m_PlayerWalls;
	std::vector<UINT> m_PlayerColour;
	std::vector<UINT> m_PlayerLight;
	UINT m_WhatLevelBefore;
	std::vector <UINT> m_Levels;
	
	
	
	 std::vector<UINT> m_SelectionShips;

	std::vector<UINT> m_Buttons;

	std::vector <std::vector<UINT>>m_MeshPlayerWall;//borde kunna ha olika väggar(olika meshar då) om man vill
	std::vector <std::vector<UINT>> m_MeshLevels;
	std::vector<std::vector<UINT>> m_MeshShips;

	std::vector<XMFLOAT3> m_Colours;

	//texturelists
	std::vector <UINT> m_ShipTexture;
	std::vector <UINT> m_LevelTexture;
	std::vector <UINT> m_PlayerWallTexture;
	GraphicEngine* m_GraphicEngine;

	UINT m_CameraID[4];

	XMMATRIX t_Mats;
	std::vector<XMFLOAT4X4> m_SelectionShipMatrix;
	static GraphicHandle* m_Singleton;
	//void CreateShip(int p_PlayerIndex, XMFLOAT3 p_Color, CXMMATRIX p_PlayerWorld,int p_ShipIndex);
	void CreateLight(int p_PlayerIndex,XMFLOAT3 p_Color,UINT p_ObjectId, LightStruct &p_LightStruct);
public:


	static GraphicHandle* GetInstance();


	void Initialize(UINT p_Width, UINT p_Height, HWND p_Handle, std::vector <std::string> p_LevelNames);
	void DrawGame();
	void ChangeHUDObject(int p_hudID, int p_hudObjID, int change());
	void UpdateDynamicLight(UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	void UpdatePlayer(int p_playerID,CXMMATRIX p_PlayerMatrix,CXMMATRIX p_CameraMatrix);
	void UpdateCamera(UINT p_CameraID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY); //for change if we are to use quaternions, else just need to add the "jaw"s =D=D
	void UpdateSelectVehicle(float p_DeltaTime);
	void CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID); //inte klar alls
	void CreateShipForGame(std::vector<int> p_WhatVehicle,std::vector<XMMATRIX> p_PlayerWorld,std::vector<XMFLOAT3>p_PlayerColor,CXMMATRIX p_LevelWorld,XMFLOAT3 p_LevelColor);
	void SelectVehicle();
	void ChangeLevelSelection(int p_WhatLevel);
	void UpdateCameraSelectLevel(int p_WhatLevel,int p_TheRotation);
	void UpdateCameraVehicleSelection(UINT p_CameraLogicID,float p_LookingAtWhatVehicle);
	void JohnSetCamera(CXMMATRIX p_World, UINT p_CameraLogicID);
	int GetAmountOfVehicles();
	int GetAmountOfLevels();
	void SetFullScreen(bool p_IsFullScreen);
	void Cleanup();
	void SetColourAndVehicle(std::vector<UINT> p_PlayerColour,std::vector<UINT> p_PlayerVehicle);
	void ChangeTexture(UINT p_HUDIDObj);
	void SetAmountOfPlayers(int p_NrOfPlayers);
	void SetCameraVehicleSelection(UINT p_CameraLogicID);
	void InitializeShip(std::string p_ShipStringName, UINT p_Texture);
	void InitializeLevel(std::string p_LevelStringName, UINT p_Texture);
	void RemoveLevelDraw(int p_RemoveLevelDraw);
	void AddLevelDraw(int p_AddLevelDraw);
	void AddSelectionDraw();
	void RemoveSelectionDraw();
	
	UINT CreateWall(int p_WhatWall,CXMMATRIX p_PlayerWallWorld,int p_WhatPlayer);
	void InitializeWall(std::string p_PlayerWallStringName, UINT p_Texture);


	void SetViewportAmount(UINT p_ViewportAmount);
	void LoadTexture(const wchar_t* p_FileName, UINT &o_TextureID);

	
	void CreateDrawObject(std::vector <UINT> p_UINTMeshLista, CXMMATRIX p_World,XMFLOAT3 p_Colour,UINT & o_ObjectID,bool p_ShouldItBeDrawn);
	void RemoveObject(UINT p_RemoveAt);


	//hud functions
	void CreateHUD(UINT &o_HUDID);
	void CreateHUDObject(XMFLOAT2 p_Position, XMFLOAT2 p_Offset, int p_TextureID1, int p_TextureID2, UINT &o_HudObjectID);
	void CreateHudTemplate(std::vector<UINT> p_HudObjects, UINT &o_TemplateID);
	void CreateHudFromTemplate(UINT p_HudTemplate, XMFLOAT3 p_Color, std::vector<XMFLOAT2> p_BarOffsets, UINT &o_HudID);
	void UseHud(UINT p_ViewportID, UINT p_HudID);
	void ChangeHudObjectTexture(UINT p_hudID, UINT p_objectID, bool p_useFrontTexture);
};