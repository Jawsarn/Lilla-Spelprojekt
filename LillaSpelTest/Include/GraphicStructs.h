
#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureUV;
};

//point Light
struct Light
{
	XMFLOAT3 position;
	float radius;
	XMFLOAT3 color;
	float filler;

	Light()
	{}
	Light(XMFLOAT3 p_Pos, float p_Rad, XMFLOAT3 p_Col, float p_Fil)
	{
		position = p_Pos;
		radius = p_Rad;
		color = p_Col;
		filler = p_Fil;
	}
	Light operator=(Light &p_in)
	{
		position = p_in.position;
		radius = p_in.radius;
		color = p_in.color;
		filler = p_in.filler;
		return *this;
	}
};
//spot light

struct SpotLight
{
	XMFLOAT3 position;
	float radius;

	XMFLOAT3 color;
	float filler;

	XMFLOAT3 direction;
	float filler2;

	SpotLight()
	{}
	SpotLight(XMFLOAT3 p_Pos, float p_Rad, XMFLOAT3 p_Col, float p_Fil, XMFLOAT3 p_Dir, float p_Fil2)
	{
		position = p_Pos;
		radius = p_Rad;
		color = p_Col;
		filler = p_Fil;
		direction = p_Dir;
		filler2 = p_Fil2;
	}
	SpotLight operator=(SpotLight &p_in)
	{
		position = p_in.position;
		radius = p_in.radius;
		color = p_in.color;
		filler = p_in.filler;
		direction = p_in.direction;
		filler2 = p_in.filler2;

		return *this;
	}
};



struct VertexBufferWithNOV
{
	ID3D11Buffer* vertexBuffer;
	UINT numberOfVertices;
};


struct DrawPiece
{
	UINT vertexBufferID; //VertexBufferWithNOV
	int diffuseTID;
	int normalTID;
	UINT shaderProgramID;
};

struct DrawObject
{
	std::vector<UINT> piecesID; //=D=D=D=D
	std::vector<UINT> lightID;
	std::vector<Light> lightWorld;	//lighteruuu
	//std::vector<transparentID>?
	std::vector<UINT> particleSystem;
	XMFLOAT3 color;
	XMFLOAT4X4 worldMatrix;
};

struct ShaderProgram
{
	int inputLayout;
	int vertexShader;
	int hullShader;
	int domainShader;
	int geometryShader;
	int pixelShader;
};

struct PerFramebuffer
{
	XMMATRIX View[4];
	XMMATRIX Projection[4];
	XMFLOAT4 EyePosition[4];
	XMFLOAT3 fillers3;
	float NumberOfViewports;
};

struct PerObjectBuffer //updates for each object that needs this
{
	XMMATRIX World;
	XMFLOAT3 Color;
	float typeOfObject; //with types of textures etc
};

struct PerComputeBuffer
{
	XMFLOAT2 screenDimensions;
	XMFLOAT2 camNearFar;
};




///HUD

struct HudVertex
{
	XMFLOAT2 position;
	XMFLOAT2 offset;
};

struct HudObject
{
	UINT vertexBufferID;
	std::vector<UINT> textures;
};

struct HudTemplate
{
	std::vector<UINT> hudObjects;
};

struct Hud
{
	UINT templateID;
	XMFLOAT3 color;
	std::vector<UINT> activeTexture;
	std::vector<XMFLOAT2> barOffsets;
};

struct HudConstantBuffer
{
	XMFLOAT3 color;
	float viewport;

	XMFLOAT2 barOffset; 
	XMFLOAT2 filler2;
};


//particle struct

struct Particle //change in the input layout
{
	XMFLOAT3 InitialPosOneW;
	XMFLOAT3 InitialPosTwoW;
	XMFLOAT3 ParentVector;
	XMFLOAT2 SizeW;
	float AgeOne;
	float AgeTwo;
	float Lifespan;
	UINT Type;
	
	Particle()
	{}
	Particle(XMFLOAT3 p_InitPos, XMFLOAT3 p_InitVel, XMFLOAT3 p_ParentVector,XMFLOAT2 p_SizeW, float p_AgeOne, float p_AgeTwo,float p_Lifespan, UINT p_Type)
	{
		InitialPosOneW = p_InitPos;
		InitialPosTwoW = p_InitVel;
		ParentVector = p_ParentVector;
		SizeW = p_SizeW;
		AgeOne = p_AgeOne;
		AgeTwo = p_AgeTwo;
		Lifespan = p_Lifespan;
		Type = p_Type;
	}
	Particle operator=(Particle &p_in)
	{
		InitialPosOneW = p_in.InitialPosOneW;
		InitialPosTwoW = p_in.InitialPosTwoW;
		ParentVector = p_in.ParentVector;
		SizeW = p_in.SizeW;
		AgeOne = p_in.AgeOne;
		AgeTwo = p_in.AgeTwo;
		Lifespan = p_in.Lifespan;
		Type = p_in.Type;
	}
};


struct InstancedGroup
{
	UINT DrawPieceID;
	int numOfNewMatrixes;
	ID3D11Buffer* InstanceBuffer;
	XMFLOAT3 Color;
	std::vector<XMFLOAT4X4> WorldMatrixes;
};