#include "GraphicHudShaderHelper.fx"

struct VS_OUTPUT
{
	float2 position : POSITION;
	float2 size		: SIZE;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCORD;
	uint viewport	: SV_ViewportArrayIndex;
};

[maxvertexcount(6)]
void GS(point VS_OUTPUT input[1], inout TriangleStream <GS_OUTPUT> triangle_stream)
{
	GS_OUTPUT o1;

	o1.position = float4(input[0].position.x - input[0].size.x, input[0].position.y + input[0].size.y, 0.0f, 1.0f); //topleft
	o1.tex = float2(0,0);
	o1.viewport = activeViewport;
	triangle_stream.Append(o1);

	GS_OUTPUT o2;
	o2.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y + input[0].size.y, 0.0f, 1.0f); //topright
	o2.tex = float2(1,0);
	o2.viewport = activeViewport;
	triangle_stream.Append(o2);

	GS_OUTPUT o3;
	o3.position = float4(input[0].position.x - input[0].size.x, input[0].position.y - input[0].size.y, 0.0f, 1.0f); //botleft
	o3.tex = float2(0,1);
	o3.viewport = activeViewport;
	triangle_stream.Append(o3);
	triangle_stream.RestartStrip();

	GS_OUTPUT o4;
	o4.position = float4(input[0].position.x - input[0].size.x, input[0].position.y - input[0].size.y, 0.0f, 1.0f); //botleft
	o4.tex = float2(0,1);
	o4.viewport = activeViewport;
	triangle_stream.Append(o4);

	GS_OUTPUT o5;
	o5.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y + input[0].size.y, 0.0f, 1.0f); //topright
	o5.tex = float2(1,0);
	o5.viewport = activeViewport;
	triangle_stream.Append(o5);

	GS_OUTPUT o6;
	o6.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y - input[0].size.y, 0.0f, 1.0f); //botright
	o6.tex = float2(1,1);
	o6.viewport = activeViewport;
	triangle_stream.Append(o6);
	triangle_stream.RestartStrip();
}