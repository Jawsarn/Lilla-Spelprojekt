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
	HRESULT ParticleSystem::CreateParticleSystem(UINT p_EffectType, const wchar_t * p_FileName , UINT p_StartBufferID, UINT p_MaxParticles, XMFLOAT3 p_Color, float p_SpawnTimer , float p_ParticleLifeSpan, float p_SpawnAmount, XMFLOAT2 p_ParticleInitSize, CXMMATRIX p_WorldMatrix,UINT &systemID);
//	void Reset(UINT systemID);
	HRESULT UpdateParticleSystemMatrix(UINT p_ParticleSystemID, CXMMATRIX p_WorldMatrix);
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
		XMFLOAT3 color;
		bool firstrun;


		//all data here
		float spawnTimer;
		float particleLifeSpan;
		float spawnAmount;
		XMFLOAT2 particleInitSize;

		XMFLOAT4X4 worldMatrix;

		// add here
	};


	struct CPerEffectBuffer
	{
		float deltaTime; //done
		float spawnAmount; //done
		float particleLifeSpan; //done
		float spawnTimer; //done

		XMFLOAT2 initialSize; //done
		XMFLOAT2 fillers56;

		XMMATRIX worldMatrix;
	};

	/*

	maybe go with xmmatrix world , and keep the positions for emitters cause they're not visisble =S

	*/




	std::vector<ParticleShaderProgram> m_ParticleShaderPrograms;

	std::vector<ParticleEffectSystem> m_ParticleEffectSystems;


	//Buffers?
	ID3D11Buffer*				m_PerEffectBuffer;

	ID3D11DepthStencilState* m_NoWriteDepthState;
	ID3D11DepthStencilState* m_DepthOff;
	ID3D11BlendState* m_BlendOn;
	ID3D11BlendState* m_BlendOff;

	ID3D11Buffer* m_PerFrameBuffer;
	ID3D11Buffer* m_PerObjectBuffer;
};

