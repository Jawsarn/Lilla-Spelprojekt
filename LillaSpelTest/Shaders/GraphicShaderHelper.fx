

cbuffer PerFrameBuffer : register(c0)
{
	matrix View
	matrix Projection;
	matrix ViewProjection;
	float4 EyesPos;
}

cbuffer PerObjectBuffer	:register(c1)
{
	matrix World;
	 
}

