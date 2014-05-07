//#include <d3d11_1.h>
//#include "MathHelper.h"
//#include "ShaderLoader.h"
//#include "Camera.h"
//
//#pragma once
//
//
//class ParticleSystem
//{
//public:
//	
//
//	
//	static ParticleSystem* GetInstance();
//
//	HRESULT Initialize( ID3D11Device* inDevice, ID3D11DeviceContext* inDeviceContext, ID3D11DepthStencilState* onDepthState, ID3D11DepthStencilState* offDepthState, ID3D11BlendState* onBlendState, ID3D11BlendState* offBlendState, ID3D11Buffer* perObjBuff);
//
//	HRESULT CreateStartParticlesBuffer(std::vector<Particle> startParticles, UINT &bufferID);
//	void CreateCBsetup(XMFLOAT3 worldAcceler, float flareEmitNumber, XMFLOAT3 emitDirection, float initSpawnAmount, float particleLifeSpan, XMFLOAT2 initialSize, UINT &dataID);
//	HRESULT CreateParticleSystem(UINT type, UINT textureID , UINT startBufferID,CXMMATRIX world,UINT cbID, UINT maxParticles,UINT &systemID);
//	HRESULT LoadTexture(const wchar_t * fileName, UINT &outID);
//	void Reset(UINT systemID);
//	void Draw(float dt, float gt);
//
//	
//private:
//	
//	ParticleSystem();
//	~ParticleSystem(void);
//	
//	HRESULT CreateRandomTexture1DSRV();
//	HRESULT CreateConstantBuffer();
//	HRESULT CreateShaders();
//	void UpdateParticles(UINT id);
//	void DrawParticles(UINT id);
//	void Cleanup();
//
//	static ParticleSystem* singleton;
//
//	ID3D11Device* device;
//	ID3D11DeviceContext* deviceContext;
//	ShaderLoader*				shaderLoader;
//
//	ID3D11ShaderResourceView*	randomTexSRV;
//
//
//	struct VertexBufferWithNumOfVert
//	{
//		ID3D11Buffer* vertexBuffer;
//		UINT numOfVertices;
//	};
//
//	std::vector<ID3D11VertexShader*> vertexShaders;
//	std::vector<ID3D11GeometryShader*> geometryShaders;
//	std::vector<ID3D11PixelShader*> pixelShaders;
//	std::vector<ID3D11InputLayout*> inputLayouts;
//	std::vector<ID3D11ShaderResourceView*> textureViews;
//	std::vector<VertexBufferWithNumOfVert> initVertexBuffersWithNum;
//	std::vector<ID3D11Buffer*> particleVertexBuffer;
//
//	struct ParticleShaderProgram
//	{
//		UINT updateVertexShader;
//		UINT updateGeometryShader;
//		UINT drawVertexShader;
//		UINT drawGeometryShader;
//		UINT drawPixelShader;
//		UINT updateInputLayout;
//		UINT drawInputLayout;
//	};
//
//	struct ParticleEffectSystem
//	{
//		UINT programID;
//		UINT textureID;
//		UINT startBufferID;
//		UINT drawVertexBufferID;
//		UINT updateVertexBufferID;
//		XMMATRIX world;
//		UINT perEffectDataID;
//		bool firstrun;
//		// add here
//	};
//
//	
//	struct CPerEffectBuffer
//	{
//		// for when the emit position/direction is varying
//		XMFLOAT3 worldAcceler;
//		float flareEmitNumber;
//		
//		XMFLOAT3 emitDirection;
//		float initSpawnAmount;
//
//		float particleLifeSpan;
//		XMFLOAT2 initialSize;
//		float filler;
//	};
//
//	struct CPerFrameParticleBuffer
//	{
//		float deltaTime;
//		float gametime;
//		XMFLOAT2 fillers;
//	};
//
//	std::vector<ParticleShaderProgram> particleShaderPrograms;
//
//	std::vector<ParticleEffectSystem> particleEffectSystems;
//
//	std::vector<CPerEffectBuffer> perEffectData;	
//
//
//	ID3D11Buffer*				perEffectBuffer;
//	ID3D11Buffer*				perFrameBuffer;
//	
//	ID3D11DepthStencilState* depthOn;
//	ID3D11DepthStencilState* depthOff;
//	ID3D11BlendState* blendOn;
//	ID3D11BlendState* blendOff;
//	ID3D11Buffer*	perObjBuffer;
//
//	bool firstRun;
//
//};
//
