#include <d3d11_1.h>
#include "ShaderLoader.h"
#include <vector>
#include <DirectXMath.h>
#include "DDSTextureLoader.h"
#include "GraphicStructs.h"

#pragma once

using namespace DirectX;

class ParticleSystem
{
public:

	static ParticleSystem* GetInstance();

	HRESULT Initialize( ID3D11Device* p_Device, ID3D11DeviceContext* p_DeviceContext, ID3D11DepthStencilState* p_NoWriteDepthState, ID3D11DepthStencilState* p_OffDepthState, ID3D11BlendState* p_OnBlendState, ID3D11BlendState* p_OffBlendState, ID3D11Buffer* p_PerFrameBuffer ,ID3D11Buffer* p_PerObjectBuffer );

	HRESULT CreateInitParticlesBuffer(std::vector<Particle> p_StartParticles, UINT &o_BufferID);
	void CreateCBsetup(XMFLOAT3 p_SpawnPosition, XMFLOAT3 emitDirection, float initSpawnAmount, float particleLifeSpan, XMFLOAT2 initialSize, float p_SpawnTime, UINT &dataID);
	HRESULT CreateParticleSystem(UINT p_EffectType, const wchar_t * p_FileName , UINT p_StartBufferID, XMFLOAT3 p_ObjectPosition, UINT p_DataID, UINT p_MaxParticles, XMFLOAT3 p_Color, UINT &systemID);
//	void Reset(UINT systemID);
	HRESULT UpdatePositionOnCBsetup(UINT p_ParticleSystemID, CXMMATRIX p_WorldMatrix);
	void Draw(float dt);


private:

	ParticleSystem();
	~ParticleSystem(void);

	HRESULT CreateRandomTexture1DSRV();
	float RandF();
	float RandF(float a, float b);

	XMVECTOR RandUnitVec3();

	HRESULT CreateConstantBuffer();
	HRESULT CreateShaders();
	void UpdateParticles(UINT id);
	void DrawParticles(UINT id);
//	void Cleanup();

	static ParticleSystem* singleton;

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ShaderLoader* m_ShaderLoader;

	ID3D11ShaderResourceView*	m_RandomTexSRV;


	struct VertexBufferWithNumOfVert
	{
		ID3D11Buffer* vertexBuffer;
		UINT numOfVertices;
	};

	std::vector<ID3D11VertexShader*> m_VertexShaders;
	std::vector<ID3D11GeometryShader*> m_GeometryShaders;
	std::vector<ID3D11PixelShader*> m_PixelShaders;
	std::vector<ID3D11InputLayout*> m_InputLayouts;

	std::vector<ID3D11ShaderResourceView*> m_TextureViews;
	std::vector<VertexBufferWithNumOfVert> m_InitParticleVertexBuffersWithNum;
	std::vector<ID3D11Buffer*> m_ParticleVertexBuffer;

	struct ParticleShaderProgram
	{
		UINT updateVertexShader;
		UINT updateGeometryShader;
		UINT drawVertexShader;
		UINT drawGeometryShader;
		UINT drawPixelShader;
		UINT updateInputLayout;
		UINT drawInputLayout;
	};

	struct ParticleEffectSystem
	{
		UINT programID;
		UINT textureID;
		UINT startBufferID;
		UINT drawVertexBufferID;
		UINT updateVertexBufferID;
		XMFLOAT3 objectPosition;
		XMFLOAT3 color;
		UINT perEffectDataID; //maybe place data  here anyway?
		bool firstrun;
		// add here
	};


	struct CPerEffectBuffer
	{
		// for when the emit position/direction is varying
		XMFLOAT3 spawnPosition;
		float deltaTime;

		float particleLifeSpan;
		float spawnTime;

		XMFLOAT2 initialSize;

		
		float initSpawnAmount;
		XMFLOAT3 emitDirection;
		
	};





	std::vector<ParticleShaderProgram> m_ParticleShaderPrograms;

	std::vector<ParticleEffectSystem> m_ParticleEffectSystems;

	std::vector<CPerEffectBuffer> m_PerEffectData;	

	//Buffers?
	ID3D11Buffer*				m_PerEffectBuffer;

	ID3D11DepthStencilState* m_NoWriteDepthState;
	ID3D11DepthStencilState* m_DepthOff;
	ID3D11BlendState* m_BlendOn;
	ID3D11BlendState* m_BlendOff;

	ID3D11Buffer* m_PerFrameBuffer;
	ID3D11Buffer* m_PerObjectBuffer;
};

