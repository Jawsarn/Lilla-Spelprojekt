#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "ShaderLoader.h"
#include <vector>
#include <map>
#include "GraphicStructs.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "ParticleSystem.h"

using namespace DirectX;





#define THREAD_BLOCK_DIMENSIONS 16
#define THREAD_VERTBLURR_DIMENSION 256
#define THREAD_HORBLURR_DIMENSION 256

#define MAX_NUM_OF_LIGHTS 1024

#define MAX_INSTANCEBUFFER_SIZE 4000;

class GraphicEngine
{
public:
	static GraphicEngine* GetInstance();
	
	//wants the width and height of actual writing area
	HRESULT Initialize( UINT p_Width, UINT p_Height, HWND handleWindow );


	
	
	enum TextureType{DIFFUSE,NORMAL};

	//object
	HRESULT LoadMesh(std::string p_FileName, std::vector<UINT> &o_DrawPieceIDs);
	HRESULT AddTextureToDrawPiece(UINT p_DrawPieceID, UINT p_TextureID,TextureType p_TextureType);
	HRESULT CreateDrawObject(std::vector<UINT> p_DrawPieceIDs, XMFLOAT4X4 p_World, XMFLOAT3 p_Color, bool addToDrawNow, bool instancedDraw, UINT &o_ObjectID);
	HRESULT AddObjectLight(UINT p_ObjectID ,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float radius, UINT &o_LightID);
	void AddObjectParticleSystem(UINT p_Object, UINT p_ParticleSystem);
	HRESULT ChangeObjectsLight(UINT p_ObjectID, UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	HRESULT MoveObject(UINT p_ObjectID, CXMMATRIX p_Matrix);
	HRESULT UpdateDrawObjectColor(UINT p_ObjectID, XMFLOAT3 p_Color);
	void RemoveObject(UINT p_ObjectID);
	void RemoveObjectFromDrawing(UINT p_ObjectID);
	void AddObjectToDrawing(UINT p_ObjectID);
	HRESULT AddObjectToInstanced(UINT p_ObjectID);
	void UpdateInstanceBuffer(UINT bufferID);

	//texture functions
	HRESULT LoadTexture(const wchar_t * p_FileName, UINT &o_TextureID);


	//light funcs
	void CreateStaticLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius, UINT &o_LightID);
	HRESULT CreateDynamicLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius, UINT &o_LightID);
	HRESULT UpdateDynamicLight(UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius);
	void RestLights();

	//hud functions //not yet implemented
	void CreateHudTemplate(std::vector<UINT> p_ObjectIDs, UINT &o_HudID);
	HRESULT CreateHudObject(XMFLOAT2 p_Position, XMFLOAT2 p_Offset, std::vector<UINT> p_Textures, UINT &o_HudObjectID);

	HRESULT CreateHudFromTemplate(UINT p_HudTemplateID,  XMFLOAT3 p_Color, std::vector<XMFLOAT2> barOffsets ,UINT &o_HudID);
	void UseHud(UINT p_Viewport, UINT p_HudI);
	void ChangeTextureOnHudObject(UINT p_HudID, UINT p_HudListID, UINT activeTexture);
	void UpdateBarOffset(UINT p_HudID, UINT p_HudListID, XMFLOAT2 barOffsets);

	//camera funcs
	HRESULT CreateCamera( XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID);
	HRESULT MoveCamera(UINT p_CameraID, float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY);
	HRESULT SetCamera(UINT p_CameraID, XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up);
	HRESULT SetCamera(UINT p_CameraID, CXMMATRIX p_Matrix);
	void UpdateLens(UINT p_CameraID, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ);
	void UseCamera(UINT p_ViewPortID, UINT p_CameraID);

	//viewport function
	void SetViewportAmount(int p_NumOfViewports);

	//particlesystem functions
	void CreateParticleSystem(UINT p_EffectType, const wchar_t * p_FileName, UINT p_StartBufferID, UINT p_MaxParticles, XMFLOAT3 p_Color , float p_SpawnTimer, float p_ParticleLifeSpan, float p_SpawnAmount, XMFLOAT2 p_ParticleInitSize, float p_Speed, float p_EngineSpeed, XMFLOAT4 p_EmitPos, CXMMATRIX p_WorldMatrix, UINT &o_SystemID );
	void UpdateParticleCB(UINT p_DataID, XMFLOAT3 p_WorldAcceler, float p_FlareEmitNumber, XMFLOAT3 p_EmitDirection, float p_InitSpawnAmount, float p_ParticleLifeSpan, XMFLOAT2 p_InitialSize);
	void CreateInitParticleBuffer(std::vector<Particle> startParticles, UINT &bufferID);
	void RemoveParticleSystem(UINT p_SystemID);
	void UpdateParticleSystem(UINT p_SystemID, float p_SpawnTimer, float p_ParticleLifespan, XMFLOAT2 p_ParticleInitSize, float p_Speed, float p_EngineSpeed );
	void DeactivateObjectsParticleSystems(UINT p_ObjectID);
	void ActivateObjectsParticleSystems(UINT p_ObjectID);
	void DeactivateParticleSystem(UINT p_SystemID);
	void ActivateParticleSystem(UINT p_SystemID);



	//skymap functions
	void SetSkymap(UINT p_TextureID);
	
	//void RemoveFromDrawObjects();
	//void AddToDrawObject();
	void DrawGame(float p_DeltaTime);
	void DrawMenu();
	void DrawSkyMap();
	
	//swap full screen
	void SetFullscreenState(bool p_IsFullScreen);

	//cleanup
	void Cleanup();


	void MasterClear();


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
	void DrawOpaqueInstancedObjects();
	void SetShaderProgram(ShaderProgram p_Program);
	void SetTextures(DrawPiece p_DrawPiece);
	void ComputeTileDeferredLightning();
	void UpdateLightBuffer();
	void DrawHud();
	void ComputeGlow();

	UINT CheckProgram(DrawPiece p_Piece);

	void UpdateInstanceBuffer();
	

	static GraphicEngine* singleton;

	MeshLoader* m_MeshLoader;
	//initialize and handles
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Device1*			m_Device1;
	ID3D11DeviceContext1*	m_DeviceContext1;
	IDXGISwapChain*			m_SwapChain;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;

	//basic rendering stuff
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
	ID3D11DepthStencilState*	m_LessEqualDepthState;
	ID3D11SamplerState*			m_SamplerStateWrap;
	ID3D11SamplerState*			m_SamplerStateLinearWrap;

	//backbuffer uav
	ID3D11UnorderedAccessView* m_BackBufferUAV;

	//gbuffers diffuseSpec/normal + gloooowie
	ID3D11ShaderResourceView* m_GbufferShaderResource[3];
	ID3D11RenderTargetView* m_GbufferTargetViews[3];

	ID3D11UnorderedAccessView* m_GbufferGlowmapUAV;

	//aaand the blur buffers/glow
	ID3D11ShaderResourceView*	m_BlurShaderResource;
	ID3D11UnorderedAccessView* m_BlurBufferUAV;


	UINT m_Width;
	UINT m_Height;

	//shader loader
	ShaderLoader* m_ShaderLoader;

	//particleSystem
	ParticleSystem* m_ParticleSystem;

	//objects lights camarsas
	std::map<UINT, DrawObject*> m_DrawObjects;
	std::map<UINT, DrawObject*> m_ObjectsOnDrawingScheme;

	std::map<UINT, Light*> m_DynamicLights;
	//std::vector<Camera*> m_Cameras;
	std::map<UINT, Camera*> m_Cameras;

	
	std::vector<DrawPiece> m_DrawPieces;
	std::vector<ID3D11ShaderResourceView*> m_Textures;
	std::vector<Light> m_StaticLights;
	int m_CurrentNumOfLights;
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
	std::vector<ID3D11InputLayout*> m_InputLayouts;
	
	//vertexbuffer
	std::vector<VertexBufferWithNOV> m_VertexBuffers;

	//constant buffers
	ID3D11Buffer* m_PerFrameBuffer;
	ID3D11Buffer* m_PerObjectBuffer;
	ID3D11Buffer* m_PerComputeBuffer;
	ID3D11Buffer* m_HudConstantBuffer;

	//light bufferu
	ID3D11Buffer*				m_LightBuffer;
	ID3D11ShaderResourceView*	m_LightBufferSRV;

	//hudss
	std::vector<HudObject> m_HudObjects;

	std::vector<HudTemplate> m_HudTemplates;
	std::map<UINT, Hud*> m_Huds;
	int m_ViewportHud[4];



	std::vector<InstancedGroup> m_InstancedList;

	//number of viewports active
	float m_NumberOfViewports;

	UINT m_SkymapBufferID;
	UINT m_SkymapDrawObjectID;
	int counter;
};

