#include "GraphicParticleHelper.fx"

struct VS_OUTPUT
{
	float3 PosW : POSITION;
	float2 Size : SIZE;
	float Age : AGE;
	float Lifespan :LIFESPAN;
	uint Type : TYPE; 
};

VS_OUTPUT VS(Particle vin)
{
	VS_OUTPUT vout;
	float t = vin.Age;

	// constant acceleration equation
	vout.PosW = /*0.5f*t*t*worldAcceler + */t*vin.InitialVelW + vin.InitialPosW;
	vout.Age = vin.Age;
	vout.Lifespan = vin.Lifespan;
	vout.Type = vin.Type;
	vout.Size = vin.SizeW;
	return vout;
}