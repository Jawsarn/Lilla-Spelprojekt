#include "GraphicParticleHelper.fx"




[maxvertexcount(90)]
void GS( point Particle input[1],  inout PointStream< Particle > ptStream )
{
	input[0].Age += deltaTime;
	float t = input[0].Age;
	float d = input[0].Lifespan;
	//float3 pos = 0.5f*t*t*worldAcceler + t*input[0].InitialVelW + input[0].InitialPosW;
	float3 vel = input[0].InitialVelW;

	if (input[0].Type > 0)
	{
		if (t >= 0.5f) //may swap this to something
		{
			for (int i = 0; i < initSpawnAmount; i++) //something
			{
				float3 random = 50*RandUnitVec3((float)i/(input[0].InitialPosW.x/10));
				
				Particle p;
				p.InitialPosW = spawnPosition;
				p.InitialVelW = float3(0,1,0)/*emitDirection + vel + random*/;
				p.SizeW = initialSize;
				p.Age = 0.0f;
				p.Lifespan = particleLifeSpan;
				p.Type = input[0].Type - 1;

				ptStream.Append(p);
			}

			input[0].Age = 0;
			ptStream.Append(input[0]);
		}
		else
		{
			ptStream.Append(input[0]);
		}
	}
	else if(t <= d) //may swap this to a buffered stop or a Particle wise stop
	{
		ptStream.Append(input[0]);
	}
}