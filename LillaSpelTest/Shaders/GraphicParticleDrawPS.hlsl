

cbuffer PerObjectBuffer	:register(c0)
{
	matrix World;
	float3 Color;
	float typeOfObject; //with types of textures etc
}

Texture2D diffuseTexture	: register(t0);


SamplerState wrapSampler	: register(s0);


struct GS_OUTPUT
{
	float4 PosH : SV_Position;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float Age : AGE;
	float Lifespan :LIFESPAN;
	uint Viewport	: SV_ViewportArrayIndex;
};



float4 PS(GS_OUTPUT input) : SV_TARGET
{
	float4 finalColor;
	
	finalColor = diffuseTexture.Sample(wrapSampler, input.Tex);
	//finalColor = float4(1,1,1,1);
	finalColor *= float4(Color, 1);
	
	//finalColor.w -= (input.Age / input.Lifespan);
	
	

	return finalColor;
}
