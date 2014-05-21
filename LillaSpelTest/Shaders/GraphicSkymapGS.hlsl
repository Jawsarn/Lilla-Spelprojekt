#include "GraphicShaderHelper.fx"

struct GS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct GS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float3 PositionL	: POSITION;
	uint Viewport		: SV_ViewportArrayIndex;
};

[maxvertexcount(12)]
void GS( triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> triangleStream )
{
	for (int j = 0; j < NumberOfViewports; j++)
	{
		for (uint i = 0; i < 3; i++)
		{
			GS_OUTPUT output;
			output.PositionL = input[i].Position;
			output.Position = mul(float4(input[i].Position, 1), View[j]);
			output.Position = mul(output.Position, Projection[j]);

			output.Viewport = j;
			triangleStream.Append(output);
		}
		triangleStream.RestartStrip();
	}
}