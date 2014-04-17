
struct GS_INPUT
{
	float3 position : POSITION;
	float2 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

[maxvertexcount(3)]
void GS( triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> triangleStream )
{
	for (uint i = 0; i < 3; i++)
	{
		GS_OUTPUT output;
		output.position = float4(input[i].position, 1);
		output.tex = input[i].tex;
		output.normal = input[i].normal;
		triangleStream.Append(output);
	}
}