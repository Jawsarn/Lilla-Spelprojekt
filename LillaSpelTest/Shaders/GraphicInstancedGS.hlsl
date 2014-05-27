#include "GraphicShaderHelper.fx"

struct GS_INPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct GS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float3 PositionW	: POSITION;
	float3 Normal		: NORMAL;
	float2 Tex			: TEXCOORD;
	float2 Depth		: DEPTH;
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
			output.Position = input[i].Position;
			output.PositionW = output.Position;
			output.Position = mul(output.Position, View[j]);
			output.Position = mul(output.Position, Projection[j]);
		/*	output.position = mul(output.position, ViewProjection);*/
			output.Tex = input[i].Tex;
			output.Normal = mul(float4(input[i].Normal.xyz,0), World);
			output.Depth = output.Position.zw;
			output.Viewport = j;
			triangleStream.Append(output);
		}
		triangleStream.RestartStrip();
	}
}