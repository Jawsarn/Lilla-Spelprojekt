#include "GraphicParticleHelper.fx"

cbuffer CPerEffectBuffer	:register(b0)
{
	float deltaTime; //done
	float spawnAmount; //done
	float particleLifeSpan; //done
	float spawnTimer; //done

	float2 initialSize; //done
	float2 fillers56;
		
	matrix worldMatrix;
}



Texture1D gRandomTex :register(t0);
SamplerState wrapSampler :register(s0);

float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (deltaTime + offset);
	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(wrapSampler, u, 0).xyz;
	// project onto unit sphere
	return normalize(v);
}
float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (deltaTime + offset);
	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(wrapSampler, u, 0).xyz;
	return v;
}


[maxvertexcount(90)]
void GS( point Particle input[1],  inout PointStream< Particle > ptStream )
{
	Particle origin = input[0];

	origin.Age += deltaTime;
	float t = origin.Age;
	float d = origin.Lifespan;
	//float3 pos = 0.5f*t*t*worldAcceler + t*input[0].InitialVelW + input[0].InitialPosW;
	//float3 vel = input[0].InitialVelW;

	if (origin.Type > 0)
	{
		if (t > spawnTimer) //may swap this to something
		{
			float3 spawnPos = mul(float4(input[0].InitialPosW, 1), worldMatrix).xyz;
			for (int i = 0; i < 1; i++) //something
			{
				//float3 random = 50*RandUnitVec3((float)i/(input[0].InitialPosW.x/10));
				
				Particle p;
				p.InitialPosW = spawnPos;
				p.InitialVelW = float3(0,0,0)/* + vel + random*/;
				p.SizeW = initialSize;
				p.Age = 0.0f;
				p.Lifespan = particleLifeSpan;
				p.Type = origin.Type - 1;

				ptStream.Append(p);
			}


			origin.Age = 0;
			ptStream.Append(origin);
		}
		else
		{
			ptStream.Append(origin);
		}
	}
	else if(t <= d) //may swap this to a buffered stop or a Particle wise stop
	{
		ptStream.Append(origin);
	}
}