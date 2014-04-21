#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "ShaderLoader.h"
#include <vector>
#include <map>
#include "GraphicStructs.h"
#include "Camera.h"
#include "MeshLoader.h"

using namespace DirectX;




/*NOTE:
the size you get to the engine is wrong, it should be gained from 
GetClientRect( g_hWnd, &rc );
UINT width = rc.right - rc.left;
UINT height = rc.bottom - rc.top;

requires saving of hwind and rc
*/



class GraphicEngine
{
public:
	static GraphicEngine* GetInstance();
	
	//wants the width and height of actual writing area
	HRESULT Initialize( UINT p_Width, UINT p_Height, HWND handleWindow );


	
	
	enum TextureType{DIFFUSE,NORMAL,GLOW,SPECULAR};

	//object
	HRESULT LoadMesh(std::string p_FileName, std::vector<UINT> &o_DrawPieceIDs);
	HRESULT AddTextureToDrawPiece(UINT p_DrawPieceID, UINT p_TextureID,TextureType p_TextureType);
	HRESULT CreateDrawObject(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, XMFLOAT3 p_Color, bool addToDrawNow, UINT &o_ObjectID);
	HRESULT AddObjectLight(UINT p_ObjectID ,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float radius, UINT &o_LightID);
	HRESULT ChangeObjectsLight(UINT p_ObjectID, UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	HRESULT MoveObject(UINT p_ObjectID, CXMMATRIX p_Matrix);
	
	//texture functions
	HRESULT LoadTexture(const wchar_t * p_FileName, UINT &o_TextureID);


	//light funcs
	void CreateStaticLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	HRESULT CreateDynamicLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius, UINT &o_LightID);
	HRESULT UpdateDynamicLight(UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);

	//hud functions //not yet implemented
	void AddHudObject();
	void CreatehudObject();
	void UseHud();
	void LoadHud();


	//camera funcs
	HRESULT CreateCamera( XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID);
	HRESULT MoveCamera(UINT p_CameraID, float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY);
	HRESULT SetCamera(UINT p_CameraID, XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up);
	void UseCamera(UINT p_ViewPortID, UINT p_CameraID);

	//viewport function
	void SetViewportAmount(int p_NumOfViewports);

	void CreateParticleSystem();
	
	
	void LoadPipeTerrain();
	
	
	
	//void RemoveFromDrawObjects();
	//void AddToDrawObject();
	void DrawGame();
	void DrawHud();
	

private:

	GraphicEngine();
	~GraphicEngine();

	HRESULT InitializeDriverAndVersion( HWND handleWindow);
	HRESULT InitializeRenderTargetView();
	void SetViewports();
	HRESULT CreateRasterizers();
	HRESULT InitializeDepthAndDepthStates();
	HRESULT InitializeBlendAndBlendStates();
	HRESULT InitializeShaders();
	HRESULT InitializeConstantBuffers();
	HRESULT InitializeGBuffers();
	HRESULT InitializeSamplerState();

	void UpdateFrameBuffer();
	void DrawOpaqueObjects();
	void SetShaderProgram(ShaderProgram p_Program);
	void SetTextures(DrawPiece p_DrawPiece);
	void ComputeTileDeferredLightning();
	void UpdateConstantBuffer(); //not written

	UINT CheckProgram(DrawPiece p_Piece);


	static GraphicEngine* singleton;

	MeshLoader* m_MeshLoader;

	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Device1*			m_Device1;
	ID3D11DeviceContext1*	m_DeviceContext1;
	IDXGISwapChain*			m_SwapChain;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;

	ID3D11RenderTargetView*		m_RenderTargetView;
	ID3D11Texture2D*			m_DepthStencil;
	ID3D11DepthStencilView*		m_DepthStencilView;
	ID3D11RasterizerState*		m_RasterizerStateNormal;
	ID3D11RasterizerState*		m_RasterizerStateWireframe;
	ID3D11BlendState*			m_BlendStateOn;
	ID3D11BlendState*			m_BlendStateOff;
	ID3D11DepthStencilState*	m_DepthStateOn;
	ID3D11DepthStencilState*	m_DepthStateOff;
	ID3D11DepthStencilState*	m_DepthStateNoWrite;
	ID3D11SamplerState*			m_SamplerStateWrap;

	ID3D11UnorderedAccessView* m_BackBufferUAV;
	ID3D11ShaderResourceView* m_GbufferShaderResource[3];
	ID3D11RenderTargetView* m_GbufferTargetViews[3];


	UINT m_Width;
	UINT m_Height;

	ShaderLoader* m_ShaderLoader;

	
	std::map<UINT, DrawObject*> m_DrawOjbects;
	std::map<UINT, Light*> m_DynamicLights;
	//std::vector<Camera*> m_Cameras;
	std::map<UINT, Camera*> m_Cameras;

	
	std::vector<DrawPiece> m_DrawPieces;
	std::vector<ID3D11ShaderResourceView*> m_Textures;
	std::vector<Light> m_StaticLights;
	Camera* m_ActiveCameras[4];

	//shader programs
	std::vector<ShaderProgram> m_ShaderPrograms;

	//shaders
	std::vector<ID3D11VertexShader*> m_VertexShaders;
	std::vector<ID3D11HullShader*> m_HullShaders;
	std::vector<ID3D11DomainShader*> m_DomainShaders;
	std::vector<ID3D11GeometryShader*> m_GeometryShaders;
	std::vector<ID3D11PixelShader*> m_PixelShaders;
	std::vector<ID3D11ComputeShader*> m_ComputeShaders;

	//input layout
	std::vector<ID3D11InputLayout*> inputLayouts;
	
	//vertexbuffer
	std::vector<VertexBufferWithNOV> m_VertexBuffers;

	//constant buffers
	ID3D11Buffer* m_PerFrameBuffer;
	ID3D11Buffer* m_PerObjectBuffer;

	UINT m_NumberOfViewports;
};

