
#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureUV;
};

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
	int glowTID;
	int specularTID;
	UINT shaderProgramID;
};

struct DrawObject
{
	std::vector<UINT> piecesID; //=D=D=D=D
	std::vector<Light> lights;	//lighteruuu
	//std::vector<transparentID>?
	//std::vector<ParticleEffectID>
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
	//XMFLOAT4 EyesPos[4];
	XMFLOAT3 fillers3;
	float NumberOfViewports;
};

struct PerObjectBuffer //updates for each object that needs this
{
	XMMATRIX World;
	float typeOfObject; //with types of textures etc
	XMFLOAT3 fillers;
};

struct PerComputeBuffer
{
	XMFLOAT2 screenDimensions;
	XMFLOAT2 camNearFar;
};