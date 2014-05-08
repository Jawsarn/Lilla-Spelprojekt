#include <d3d11_1.h>
#include "ShaderLoader.h"
#include <vector>
#include <DirectXMath.h>
#include "DDSTextureLoader.h"

#pragma once

using namespace DirectX;

class ParticleSystem
{
public:

	struct Particle //change in the input layout
	{
		XMFLOAT3 InitialPosW;
		XMFLOAT3 InitialVelW;
		XMFLOAT2 SizeW;
		float Age;
		float Lifespan;
		UINT Type;
	};


	static ParticleSystem* GetInstance();

	HRESULT Initialize( ID3D11Device* p_Device, ID3D11DeviceContext* p_DeviceContext, ID3D11DepthStencilState* p_OnDepthState, ID3D11DepthStencilState* p_OffDepthState, ID3D11BlendState* p_OnBlendState, ID3D11BlendState* p_OffBlendState );

	HRESULT CreateStartParticlesBuffer(std::vector<Particle> startParticles, UINT &bufferID);
//	void CreateCBsetup(XMFLOAT3 worldAcceler, float flareEmitNumber, XMFLOAT3 emitDirection, float initSpawnAmount, float particleLifeSpan, XMFLOAT2 initialSize, UINT &dataID);
	HRESULT ParticleSystem::CreateParticleSystem(UINT p_EffectType, const wchar_t * p_FileName , UINT p_StartBufferID, CXMMATRIX p_World, UINT p_DataID, UINT p_MaxParticles, UINT &systemID);
//	HRESULT LoadTexture(const wchar_t * fileName, UINT &outID);
//	void Reset(UINT systemID);
	void Draw(float dt, float gt);


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
		DirectX::XMFLOAT4X4 world;
		UINT perEffectDataID; //maybe place data  here anyway?
		bool firstrun;
		// add here
	};


	struct CPerEffectBuffer
	{
		// for when the emit position/direction is varying
		XMFLOAT3 worldAcceler;
		float flareEmitNumber;
		
		XMFLOAT3 emitDirection;
		float initSpawnAmount;

		float particleLifeSpan;
		XMFLOAT2 initialSize;
		float filler;
	};

	struct CPerFrameParticleBuffer
	{
		float deltaTime;
		float gametime;
		XMFLOAT2 fillers;
	};

	std::vector<ParticleShaderProgram> m_ParticleShaderPrograms;

	std::vector<ParticleEffectSystem> m_ParticleEffectSystems;

	std::vector<CPerEffectBuffer> m_PerEffectData;	

	//Buffers?
	ID3D11Buffer*				m_PerEffectBuffer;
	ID3D11Buffer*				m_PerFrameBuffer;

	ID3D11DepthStencilState* m_DepthOn;
	ID3D11DepthStencilState* m_DepthOff;
	ID3D11BlendState* m_BlendOn;
	ID3D11BlendState* m_BlendOff;


};

