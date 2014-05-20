#include "GraphicParticleHelper.fx"

cbuffer CPerEffectBuffer	:register(b0)
{
	float deltaTime; //done
	float spawnAmount; //done
	float particleLifeSpan; //done
	float spawnTimer; //done

	float2 initialSize; //done
	float speed;
	float engineSpeed;
	
	float4 emitPos;

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


[maxvertexcount(2)]
void GS( point Particle input[1],  inout PointStream< Particle > ptStream )
{
	Particle origin = input[0];

	origin.AgeOne += deltaTime;
	origin.AgeTwo += deltaTime;

	float t = origin.AgeTwo;
	float d = origin.Lifespan;

	if (origin.Type > 0)
	{
		origin.InitialPosOneW = mul(emitPos, worldMatrix).xyz;
		origin.AgeOne = spawnTimer - t;

		if (t > spawnTimer) //if the age form first has reached a limit, then set the second 
		{
			//finish the particle you're on and craete the new one at age  0
			origin.AgeTwo = origin.AgeOne + 1.0f;
			origin.Type = 0;

			Particle newPart;
			newPart.InitialPosOneW = mul(emitPos, worldMatrix).xyz;
			newPart.InitialPosTwoW = newPart.InitialPosOneW;
			newPart.ParVector = normalize(origin.InitialPosTwoW - origin.InitialPosOneW);
			newPart.SizeW = initialSize;
			newPart.AgeOne = 0.0f;
			newPart.AgeTwo = 0.0f;
			newPart.Lifespan = particleLifeSpan;
			newPart.Type = 1;

			ptStream.Append(newPart);
		}
		
		//change position and age of second
		

		
		//always append
		ptStream.Append(origin);
	}
	else if(t <= d) //may swap this to a buffered stop or a Particle wise stop
	{
		ptStream.Append(origin);
	}
}