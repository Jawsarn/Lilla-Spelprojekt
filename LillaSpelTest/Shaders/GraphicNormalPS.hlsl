

cbuffer PerObjectBuffer	:register(b0)
{
	matrix World;
	float3 Color;
	float typeOfObject; //with types of textures etc
}

cbuffer PerFrameBuffer :register(b1)
{
	matrix View[4];
	matrix Projection[4];
	float4 EyePosition[4];
	float3 fillers;
	float NumberOfViewports;
}



Texture2D diffuseTexture	: register(t0);
Texture2D normalGlow		: register(t1);
TextureCube cubeMap			: register(t2);


SamplerState wrapSampler	: register(s0);

struct GS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float3 PositionW	: POSITION;
	float3 Normal		: NORMAL;
	float2 Tex			: TEXCOORD;
	float2 Depth		: DEPTH;
	uint Viewport		: SV_ViewportArrayIndex;
};

struct PS_OUTPUT
{
	// example like 
	float4 Normal_Depth			:SV_TARGET0;
	float4 DiffuseColor_Spec	:SV_TARGET1;
	float4 Glowmap				:SV_TARGET2;
};


PS_OUTPUT PS(GS_OUTPUT input) : SV_TARGET
{
	PS_OUTPUT output;

	float4 diffuseColorSpecFac = diffuseTexture.Sample(wrapSampler, input.Tex);
	float4 normalGlowFac = normalGlow.Sample(wrapSampler, input.Tex);

	float depth = input.Depth.x / input.Depth.y;

	output.Normal_Depth = float4(input.Normal, depth);


	//skymap shizzle
	float3 toObj = input.PositionW - EyePosition[input.Viewport];
	float3 reflector = reflect(toObj, input.Normal);

	diffuseColorSpecFac.xyz += diffuseColorSpecFac.w * cubeMap.Sample( wrapSampler, reflector);

	float4 glowOut;
	if (normalGlowFac.w > 0.1f)
	{
		glowOut = float4(Color, 1)*(normalGlowFac.w);
		diffuseColorSpecFac = float4(Color , 0 );
	}
	else
	{
		glowOut = float4(0,0,0,1);
	}

	output.Glowmap = glowOut;

	output.DiffuseColor_Spec = diffuseColorSpecFac;

	

	return output;
}