#include "GraphicParticleHelper.fx"

struct VS_OUTPUT
{
	float3 PosW : POSITION;
	float2 Size : SIZE;
	float Age : AGE;
	uint Type : TYPE; 
};

struct GS_OUTPUT
{
	float4 PosH : SV_Position;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float Age : AGE;
	uint Viewport	: SV_ViewportArrayIndex;
};

[maxvertexcount(3)]
void GS( point VS_OUTPUT gin[1], inout LineStream< GS_OUTPUT > tristreamStream )
{
	//if (gin[0].Type == 0)
	//{
	for (int i = 0; i < 4; i++)
	{
		float3 p = gin[0].PosW;
		float2 d = gin[0].Size;

		GS_OUTPUT v0;
		v0.PosH.xyz = p;
		v0.PosH = mul(float4(v0.PosH.xyz, 1.0f), View[i]);
		v0.PosH = mul(v0.PosH, Projection[i]);
		v0.Norm = EyePosition[i] - p;
		v0.Tex = float2(0.0f, 0.0f);
		v0.Age = gin[0].Age;
		v0.Viewport = i;
		tristreamStream.Append(v0);

		GS_OUTPUT v1;
		v1.PosH.xyz = p + float3(0,-5,0);
		v1.PosH = mul(float4(v1.PosH.xyz, 1.0f), View[i]);
		v1.PosH = mul(v1.PosH, Projection[i]);
		v1.Norm = EyePosition[i] - p;
		v1.Tex = float2(1.0f, 0.0f);
		v1.Age = gin[0].Age;
		v1.Viewport = i;
		tristreamStream.Append(v1);


		/*float3 dir = normalize( eyePos - p );
		float3 right = normalize(cross(dir, float3(0,1,0)));
		float3 up = normalize(cross(right, dir));

		GS_OUTPUT v0;
		v0.PosH.xyz = gin[0].PosW + up - right;
		v0.PosH = mul(float4(v0.PosH.xyz, 1.0f), view);
		v0.PosH = mul(v0.PosH, projection);
		v0.Norm = dir;
		v0.Depth = v0.PosH.zw;
		v0.Tex = float2(0.0f, 0.0f);
		v0.Age = gin[0].Age;
		tristreamStream.Append(v0);

		GS_OUTPUT v1;
		v1.PosH.xyz = gin[0].PosW + up + right;
		v1.PosH = mul(float4(v1.PosH.xyz, 1.0f), view);
		v1.PosH = mul(v1.PosH, projection);
		v1.Norm = dir;
		v1.Depth = v1.PosH.zw;
		v1.Tex = float2(1.0f, 0.0f);
		v1.Age = gin[0].Age;
		tristreamStream.Append(v1);

		GS_OUTPUT v2;
		v2.PosH.xyz = gin[0].PosW - up - right ;
		v2.PosH = mul(float4(v2.PosH.xyz, 1.0f), view);
		v2.PosH = mul(v2.PosH, projection);
		v2.Norm = dir;
		v2.Depth = v2.PosH.zw;
		v2.Tex = float2(0.0f, 1.0f);
		v2.Age = gin[0].Age;
		tristreamStream.Append(v2);
		tristreamStream.RestartStrip();

		GS_OUTPUT v3;
		v3.PosH.xyz = gin[0].PosW - up - right;
		v3.PosH = mul(float4(v3.PosH.xyz, 1.0f), view);
		v3.PosH = mul(v3.PosH, projection);
		v3.Norm = dir;
		v3.Depth = v3.PosH.zw;
		v3.Tex = float2(0.0f, 1.0f);
		v3.Age = gin[0].Age;
		tristreamStream.Append(v3);

		GS_OUTPUT v4;
		v4.PosH.xyz = gin[0].PosW + up + right;
		v4.PosH = mul(float4(v4.PosH.xyz, 1.0f), view);
		v4.PosH = mul(v4.PosH, projection);
		v4.Norm = dir;
		v4.Depth = v4.PosH.zw;
		v4.Tex = float2(1.0f, 0.0f);
		v4.Age = gin[0].Age;
		tristreamStream.Append(v4);

		GS_OUTPUT v5;
		v5.PosH.xyz = gin[0].PosW - up + right ;
		v5.PosH = mul(float4(v5.PosH.xyz, 1.0f), view);
		v5.PosH = mul(v5.PosH, projection);
		v5.Norm = dir;
		v5.Depth = v5.PosH.zw;
		v5.Tex = float2(1.0f, 1.0f);
		v5.Age = gin[0].Age;
		tristreamStream.Append(v5);
		tristreamStream.RestartStrip();*/
	/*}*/
	}
}