

cbuffer cbSettings
{
	float gWeights[11];
	float filler;
}

cbuffer cbFixed
{
	static const int g_BlurRadius ;
}

Texture2D input;
RWTexture2D<float4> output;

#define N 256
#define CacheSize (N + 2*g_BlurRadius)
groupshared float4 g_Cache[CacheSize];

[numthreads(N, 1, 1)]
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID, int3 dispatchThreadID : SV_DispatchThreadID)
{
	//the threads that overlap the radius set the outscope values of the cached memory (the end values get duplicate)
	if(groupThreadID.x < g_BlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
	}
	if(groupThreadID.x >= N - gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = min(dispatchThreadID.x + gBlurRadius, gInput.Length.x - 1);
		gCache[groupThreadID.x+2*gBlurRadius] =	gInput[int2(x, dispatchThreadID.y)];
	}

	// Clamp out of bound samples that occur at image borders.
	gCache[groupThreadID.x+gBlurRadius] = gInput[min(dispatchThreadID.xy, gInput.Length.xy-1)];

	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//
	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.x + gBlurRadius + i;
		blurColor += gWeights[i+gBlurRadius]*gCache[k];
	}

	gOutput[dispatchThreadID.xy] = blurColor;
}