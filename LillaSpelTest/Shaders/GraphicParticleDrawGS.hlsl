#include "GraphicParticleHelper.fx"

struct VS_OUTPUT
{
	float3 PosW : POSITION;
	float2 Size : SIZE;
	float Age : AGE;
	float Lifespan :LIFESPAN;
	uint Type : TYPE; 
};

struct GS_OUTPUT
{
	float4 PosH : SV_Position;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float Age : AGE;
	float Lifespan :LIFESPAN;
	uint Viewport	: SV_ViewportArrayIndex;
};

[maxvertexcount(24)]
void GS( point VS_OUTPUT gin[1], inout TriangleStream< GS_OUTPUT > tristreamStream )
{
	if (gin[0].Type == 0)
	{
		for (int i = 0; i < NumberOfViewports; i++)
		{
			float3 p = gin[0].PosW;
			float2 d = gin[0].Size*(1 + (gin[0].Age/gin[0].Lifespan));

			float3 dir = normalize( EyePosition[i] - p );
			float3 right = normalize(cross(dir, float3(0,1,0)));
			float3 up = normalize(cross(right, dir));

			GS_OUTPUT v0;
			v0.PosH.xyz = gin[0].PosW + up*d.y - right*d.x;
			v0.PosH = mul(float4(v0.PosH.xyz, 1.0f), View[i]);
			v0.PosH = mul(v0.PosH, Projection[i]);
			v0.Norm = dir;
			v0.Viewport = i;
			v0.Tex = float2(0.0f, 0.0f);
			v0.Age = gin[0].Age;
			v0.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v0);

			GS_OUTPUT v1;
			v1.PosH.xyz = gin[0].PosW + up*d.y + right*d.x;
			v1.PosH = mul(float4(v1.PosH.xyz, 1.0f), View[i]);
			v1.PosH = mul(v1.PosH, Projection[i]);
			v1.Norm = dir;
			v1.Viewport = i;
			v1.Tex = float2(1.0f, 0.0f);
			v1.Age = gin[0].Age;
			v1.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v1);

			GS_OUTPUT v2;
			v2.PosH.xyz = gin[0].PosW - up*d.y - right*d.x ;
			v2.PosH = mul(float4(v2.PosH.xyz, 1.0f), View[i]);
			v2.PosH = mul(v2.PosH, Projection[i]);
			v2.Norm = dir;
			v2.Viewport = i;
			v2.Tex = float2(0.0f, 1.0f);
			v2.Age = gin[0].Age;
			v2.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v2);
			tristreamStream.RestartStrip();

			GS_OUTPUT v3;
			v3.PosH.xyz = gin[0].PosW - up*d.y - right*d.x;
			v3.PosH = mul(float4(v3.PosH.xyz, 1.0f), View[i]);
			v3.PosH = mul(v3.PosH, Projection[i]);
			v3.Norm = dir;
			v3.Viewport = i;
			v3.Tex = float2(0.0f, 1.0f);
			v3.Age = gin[0].Age;
			v3.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v3);

			GS_OUTPUT v4;
			v4.PosH.xyz = gin[0].PosW + up*d.y + right*d.x;
			v4.PosH = mul(float4(v4.PosH.xyz, 1.0f), View[i]);
			v4.PosH = mul(v4.PosH, Projection[i]);
			v4.Norm = dir;
			v4.Viewport = i;
			v4.Tex = float2(1.0f, 0.0f);
			v4.Age = gin[0].Age;
			v4.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v4);

			GS_OUTPUT v5;
			v5.PosH.xyz = gin[0].PosW - up*d.y + right*d.x ;
			v5.PosH = mul(float4(v5.PosH.xyz, 1.0f), View[i]);
			v5.PosH = mul(v5.PosH, Projection[i]);
			v5.Norm = dir;
			v5.Viewport = i;
			v5.Tex = float2(1.0f, 1.0f);
			v5.Age = gin[0].Age;
			v5.Lifespan = gin[0].Lifespan;
			tristreamStream.Append(v5);
			tristreamStream.RestartStrip();

		}
	}
}