
struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};



float4 PS(GS_OUTPUT input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}