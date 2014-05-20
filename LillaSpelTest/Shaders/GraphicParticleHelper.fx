
#define PT_FLARE 0
#define PT_EMITTER 1
#define PT_EM_EMITTER 2



//cbuffer CPerEffectBuffer	:register(b2)
//{
//	float3 spawnPosition;
//	float flareEmitNumber;
//
//	float3 emitDirection;
//	float initSpawnAmount;
//
//	float particleLifeSpan;
//	float spawnTime;
//	float2 initialSize;
//	
//}
//
//cbuffer CPerFrameParticleBuffer	:register(b3)
//{
//	float deltaTime;
//	float3 fillers2;
//}



struct Particle //change in the input layout
{
	float3 InitialPosOneW : POSITION0;
	float3 InitialPosTwoW : POSITION1;
	float3 ParVector : VECTOR;
	float2 SizeW : SIZE;
	float AgeOne : AGE0;
	float AgeTwo : AGE1;
	float Lifespan :LIFESPAN;
	uint Type : TYPE;
};

