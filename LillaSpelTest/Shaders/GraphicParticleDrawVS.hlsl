#include "GraphicParticleHelper.fx"

struct VS_OUTPUT
{
	float3 PosOneW : POSITION0;
	float3 PosTwoW : POSITION1;
	float3 ParVector : VECTOR;
	float2 Size : SIZE;
	float AgeOne : AGE0;
	float AgeTwo : AGE1;
	float Lifespan :LIFESPAN;
	uint Type : TYPE; 
};

VS_OUTPUT VS(Particle vin)
{
	VS_OUTPUT vout;

	// constant acceleration equation
	vout.PosOneW = vin.InitialPosOneW;
	vout.PosTwoW = vin.InitialPosTwoW;
	vout.ParVector = vin.ParVector;
	vout.AgeOne = vin.AgeOne;
	vout.AgeTwo = vin.AgeTwo;
	vout.Lifespan = vin.Lifespan;
	vout.Type = vin.Type;
	vout.Size = vin.SizeW;
	return vout;
}