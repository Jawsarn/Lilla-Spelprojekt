struct VS_INPUT
{
	float2 position : POSITION;
	float2 size		: SIZE;
};


VS_INPUT VS( VS_INPUT input )
{
	return input;
}