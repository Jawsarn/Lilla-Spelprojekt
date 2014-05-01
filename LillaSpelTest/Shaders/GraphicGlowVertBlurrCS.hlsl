
/*
So... if I sample from the big texture to these caches, I will need some kind of UV cordinates, that is, full 1/fullsize, and then mix with threads?
that means.. 1/MaxThreadsNeeded and that is, threads are... erhh.. the size of the next texture? so 1920/2 = 960, and 1080/2 = 540 , so... 1/these is the thing I need to multiply
my sampler with so I get the right cords from it...

and I just remebered that sampling wont work this way, cause it will take the sampled value of what I give it.. that is the most matching pixels on that place, so that means 
it wont be sampled on a big area instead just taking the pixel at the point I'm giving the UVs
*/

cbuffer cbSettings
{
	float g_Weights[11] =
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
	};
}

cbuffer cbFixed
{
	static const int g_BlurRadius = 5;
}

RWTexture2D<float4> g_Output : register(u0);
Texture2D g_Input : register(t1);

SamplerState g_BlurrSampler;

#define N 256
#define CacheSize (N + 2*g_BlurRadius)

groupshared float4 g_Cache[CacheSize];

[numthreads(1, N, 1)]
void CS(int3 groupThreadID : SV_GroupThreadID, int3 threadID : SV_DispatchThreadID)
{
	float2 uvDimensions = float2(1/960, 1/540);

	if(groupThreadID.y < g_BlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int y = max(threadID.y - g_BlurRadius, 0);
		g_Cache[groupThreadID.y] = g_Input.SampleLevel(g_BlurrSampler, float2(threadID.x*uvDimensions.x , y*uvDimensions.y ), 0 );
	}

	if(groupThreadID.y >= N-g_BlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int y = min(threadID.y + g_BlurRadius, g_Input.Length.y - 1);

		g_Cache[groupThreadID.y+2*g_BlurRadius] = g_Input.SampleLevel(g_BlurrSampler, float2(threadID.x*uvDimensions.x , y*uvDimensions.y ), 0 );
	}

	// Clamp out of bound samples that occur at image borders.
	g_Cache[groupThreadID.y + g_BlurRadius] = g_Input.SampleLevel(g_BlurrSampler, uvDimensions*min(threadID.xy, g_Input.Length.xy-1) , 0 );

	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//
	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for(int i = -g_BlurRadius; i <= g_BlurRadius; ++i)
	{
		int k = groupThreadID.y + g_BlurRadius + i;
		blurColor += g_Weights[i+g_BlurRadius]*g_Cache[k];
	}
	
	float4 Color;
	//float4 Color = g_Input.SampleLevel(g_BlurrSampler, uvDimensions*threadID.xy , 0 );
	if (threadID.x > 438 && threadID.x < 500)
	{
		Color = float4(1,0,0,0);
	}
	else
	{
		Color = float4(0,0,0,0);
	}
	/*Color = g_Input.SampleLevel(g_BlurrSampler, uvDimensions*threadID.xy , 0 )*0.4 +
			g_Input.SampleLevel(g_BlurrSampler, uvDimensions*float2(threadID.x + 1, threadID.y), 0 )*0.2 + 
			g_Input.SampleLevel(g_BlurrSampler, uvDimensions*float2(threadID.x + 2, threadID.y), 0 )*0.1 +
			g_Input.SampleLevel(g_BlurrSampler, uvDimensions*float2(threadID.x - 1, threadID.y), 0 )*0.2 +
			g_Input.SampleLevel(g_BlurrSampler, uvDimensions*float2(threadID.x - 2, threadID.y), 0 )*0.1;*/
			
	g_Output[threadID.xy] = Color;
}