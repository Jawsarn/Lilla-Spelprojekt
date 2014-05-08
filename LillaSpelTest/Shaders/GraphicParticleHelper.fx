#include "GraphicShaderHelper.fx"

#define PT_FLARE 0
#define PT_EMITTER 1
#define PT_EM_EMITTER 2



cbuffer CPerEffectBuffer	:register(b2)
{
	float3 worldAcceler;
	float flareEmitNumber;

	float3 emitDirection;
	float initSpawnAmount;

	float particleLifeSpan;
	float2 initialSize;
	float filler6;
}

cbuffer CPerFrameParticleBuffer	:register(b3)
{
	float deltaTime;
	float gameTime;
	float2 fillers2;
}

cbuffer EyePosition : register(b4)
{
	float4 EyePosition[4];
}


struct Particle //change in the input layout
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW : SIZE;
	float Age : AGE;
	float Lifespan :LIFESPAN;
	uint Type : TYPE;
};



//Texture1D gRandomTex :register(t1);
//
//
//float3 RandUnitVec3(float offset)
//{
//	// Use game time plus offset to sample random texture.
//	float u = (deltaTime + offset);
//	// coordinates in [-1,1]
//	float3 v = gRandomTex.SampleLevel(meshSampler, u, 0).xyz;
//	// project onto unit sphere
//	return normalize(v);
//}
//float3 RandVec3(float offset)
//{
//	// Use game time plus offset to sample random texture.
//	float u = (deltaTime + offset);
//	// coordinates in [-1,1]
//	float3 v = gRandomTex.SampleLevel(meshSampler, u, 0).xyz;
//	return v;
//}