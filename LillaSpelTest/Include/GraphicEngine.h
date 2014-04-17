#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "ShaderLoader.h"
#include <vector>


using namespace DirectX;

/*NOTE:
the size you get to the engine is wrong, it should be gained from 
GetClientRect( g_hWnd, &rc );
UINT width = rc.right - rc.left;
UINT height = rc.bottom - rc.top;

requires saving of hwind and rc


*/
struct DrawObject
{
	UINT meshID;
	UINT textureID;
	UINT normalMapTextureID;
	UINT glowMapTextureID;
};

struct Light
{

};

struct DrawEntity
{
	std::vector<DrawObject> objects;
	std::vector<Light> lights;
	//std::vector<transparentID>?
	//std::vector<ParticleEffectID>
	XMFLOAT4X4 worldMatrix;
};

class GraphicEngine
{
public:
	static GraphicEngine* GetInstance();
	
	//wants the width and height of actual writing area
	HRESULT Initialize( UINT p_Width, UINT p_Height, HWND handleWindow );


	HRESULT Loadtexture();
	
	//object
	HRESULT LoadMesh(UINT o_MeshID);
	void CreateEntity(UINT p_MeshID, UINT );
	void MoveEntity();

	//light funcs
	void MoveLigth();
	void ChangeLighting();
	void CreateLight();


	//hud functions
	void AddHudObject();
	void CreatehudObject();
	void UseHud();
	void LoadHud();


	//camera funcs
	void CreateCamera();
	void MoveCamera();
	void UseCamera();
	void RotateCamera();

	void CreateParticleSystem();
	
	
	void LoadPipeTerrain();
	
	
	
	void RemoveFromDrawObjects();
	void AddToDrawObject();
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

	void UpdateFramebuffers();
	void ComputeTileDeferredLightning();
	void UpdateConstantBuffer();



	static GraphicEngine* singleton;

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
};

