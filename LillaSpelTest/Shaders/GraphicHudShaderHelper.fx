
cbuffer HudConstantBuffer : register(c0)
{
	float3 color;
	float activeViewport;

	float2 barOffset;
	float2 filler2;
	//texture cord offset? so we can have bars??? might go to a different constantbuffer
};