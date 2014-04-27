
cbuffer HudBuffer : register(c3)
{
	float3 color;
	float filler;

	float2 barOffset;
	float NumberOfViewports;
	float filler2;
	//texture cord offset? so we can have bars??? might go to a different constantbuffer
};