

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

[numthreads(N, 1, 1)]
void CS(int3 groupThreadID : SV_GroupThreadID, int3 threadID : SV_DispatchThreadID)
{
	int2 topLeftBoxID = threadID.xy*2;
	//the threads that overlap the radius set the outscope values of the cached memory (the end values get duplicate)
	if(groupThreadID.x < g_BlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = max(threadID.x - g_BlurRadius, 0);
		g_Cache[groupThreadID.x] = g_Input[int2(x, threadID.y)];
	}
	if(groupThreadID.x >= N - g_BlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = min(threadID.x + g_BlurRadius, g_Input.Length.x - 1);
		g_Cache[groupThreadID.x+2*g_BlurRadius] =	g_Input[int2(x, threadID.y)];
	}

	// Clamp out of bound samples that occur at image borders.
	g_Cache[groupThreadID.x+g_BlurRadius] = g_Input[min(threadID.xy, g_Input.Length.xy-1)];

	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//
	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for(int i = -g_BlurRadius; i <= g_BlurRadius; ++i)
	{
		int k = groupThreadID.x + g_BlurRadius + i;
		blurColor += g_Weights[i+g_BlurRadius]*g_Cache[k];
	}

	
	g_Output[topLeftBoxID.xy] = blurColor;

	g_Output[ int2(topLeftBoxID.x + 1, topLeftBoxID.y) ] = blurColor;
	g_Output[ int2(topLeftBoxID.x, topLeftBoxID.y + 1) ] = blurColor;
	g_Output[ int2(topLeftBoxID.x + 1, topLeftBoxID.y + 1) ] = blurColor;

	/*float2 finalCord = threadID.xy;
	g_Output[topLeftBoxID.xy] = g_Input[finalCord];
	g_Output[float2(topLeftBoxID.x + 1, topLeftBoxID.y)] = g_Input[finalCord];
	g_Output[float2(topLeftBoxID.x, topLeftBoxID.y + 1)] = g_Input[finalCord];
	g_Output[float2(topLeftBoxID.x + 1, topLeftBoxID.y + 1)] = g_Input[finalCord];*/
}