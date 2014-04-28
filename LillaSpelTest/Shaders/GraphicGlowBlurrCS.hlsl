

cbuffer cbSettings
{
	float gWeights[11];
	float filler;
}

cbuffer cbFixed
{
	static const int gBlurRadius ;
}

Texture2D input;
RWTexture2D<float4> output;

#define N 256
#define CacheSize (N + 2*gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID,
int3 dispatchThreadID : SV_DispatchThreadID)
{
	//the threads that overlap the radius set the outscope values of the cached memory (the end values get duplicate)
	if(groupThreadID.x < gBlurRadius)
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

/*
[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID,
int3 dispatchThreadID : SV_DispatchThreadID)
{
//
// Fill local thread storage to reduce bandwidth. To blur
// N pixels, we will need to load N + 2*BlurRadius pixels
// due to the blur radius.
//
// This thread group runs N threads. To get the extra 2*BlurRadius
// pixels, have 2*BlurRadius threads sample an extra pixel.
if(groupThreadID.y < gBlurRadius)
{
// Clamp out of bound samples that occur at image borders.
int y = max(dispatchThreadID.y - gBlurRadius, 0);
gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
}i
f(groupThreadID.y >= N-gBlurRadius)
{
// Clamp out of bound samples that occur at image borders.
int y = min(dispatchThreadID.y + gBlurRadius, gInput.Length.y-1);
gCache[groupThreadID.y+2*gBlurRadius] =
gInput[int2(dispatchThreadID.x, y)];
}
// Clamp out of bound samples that occur at image borders.
gCache[groupThreadID.y+gBlurRadius] =
gInput[min(dispatchThreadID.xy, gInput.Length.xy-1)];
// Wait for all threads to finish.
GroupMemoryBarrierWithGroupSync();
//
// Now blur each pixel.
//
float4 blurColor = float4(0, 0, 0, 0);
[unroll]
for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
{
int k = groupThreadID.y + gBlurRadius + i;
blurColor += gWeights[i+gBlurRadius]*gCache[k];
}
gOutput[dispatchThreadID.xy] = blurColor;
*/