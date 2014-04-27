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
	GS_OUTPUT output;

	output.position = float4(input[0].position.x - input[0].size.x,		input[0].position.y - input[0].size.y, 1, 0);
	output.tex = float2(0,0);
	output.viewport = 0;
	triangle_stream.Append(output);

	output.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y - input[0].size.y, 1, 0);
	triangle_stream.Append(output);

	output.position = float4(input[0].position.x - input[0].size.x,		input[0].position.y + input[0].size.y, 1, 0);
	triangle_stream.Append(output);
	triangle_stream.RestartStrip();

	output.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y - input[0].size.y, 1, 0);
	triangle_stream.Append(output);

	output.position = float4(input[0].position.x + input[0].size.x,		input[0].position.y + input[0].size.y, 1, 0);
	triangle_stream.Append(output);

	output.position = float4(input[0].position.x - input[0].size.x,		input[0].position.y + input[0].size.y, 1, 0);
	triangle_stream.Append(output);
	triangle_stream.RestartStrip();
}