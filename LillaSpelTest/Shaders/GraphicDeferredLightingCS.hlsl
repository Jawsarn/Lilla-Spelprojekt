//#include "GraphicShaderHelper.fx"

cbuffer PerFrameBuffer : register(c0)
{
	matrix View[4];
	matrix Projection[4];
	/*float4 EyesPos[4];*/
	float3 fillers;
	float NumberOfViewports;
}

cbuffer PerComputeBuffer : register(c1)
{
	float2 screenDimensions;
	float2 camNearFar;
}

struct Light
{
	float3 position;
	float radius;
	float3 color;
	float filler;
};


//lights
StructuredBuffer<Light> lights	:register(t4);

//input textures
Texture2D<float4> Normal_Depth	:register(t1);
Texture2D<float4> DiffuseColor_Spec	:register(t2);
Texture2D<float4> Specular	:register(t3);

//output texture
RWTexture2D<float4> output	:register(u0);

//groupshared
groupshared uint visibleLightCount;
groupshared uint visibleLightIndices[1024];
groupshared uint minDepth;
groupshared uint maxDepth;


#define BLOCK_SIZE 16


///////////////////////////////////////////+/
//////////////functions/////////////////////
////////////////////////////////////////////
float4 CreateFrustrum(float4 F, float4 S)
{
	float4 outFrust;

	outFrust.xyz = normalize( cross( F.xyz, S.xyz) );

	outFrust.w = 0;

	return outFrust;
}



float3 DirectIllumination(float3 pos, float3 norm , Light light, float inSpec,int viewport)
{
	float3 lightPos = mul(float4(light.position,1),View[viewport]).xyz;

	float3 lightVec = lightPos - pos;

	float d = length(lightVec);
	if(d > light.radius )
	{
		return float3(0,0,0);
	}

	lightVec /= d;
	float diffuseFactor = dot(lightVec, norm);

	if(diffuseFactor < 0.0f)
	{
		return float3(0,0,0);
	}

	float att = pow(max(0.1, 1.0 - (d / light.radius)), 2);

	float3 toEye = -pos;
	float3 v = reflect(-lightVec, norm);
	float specFactor = pow(max(dot(v,toEye), 0.0f), 100)*inSpec;

	return (light.color * att * (diffuseFactor + specFactor));
}

struct PixelData
{
	float3 normalView;
	float3 positionView;
};

float3 ReconstructPosViewFromDepth(float2 screenPos, float depth, int viewport)
{
	float2 screenSpaceRay = float2(screenPos.x / Projection[viewport]._11, screenPos.y / Projection[viewport]._22); //the scale variables from linear distance


	float3 posView;
	posView.z = depth;

	posView.xy = screenSpaceRay.xy * posView.z; //multiply depth with the scale position, to get the position

	return posView;
}

PixelData GetPixelData(uint2 globalCord, int viewport)
{
	PixelData output;
	
	//get screen dimensions from buffer


	//get depth
	float depth = Normal_Depth[globalCord].w;
	float3 normal = Normal_Depth[globalCord].xyz;

	//might be other dimensions, but it should work as this as well if it's the screen dim, might be sample dimensions form the SSAO
	float2 screenPixelOffset = float2(2.0f, -2.0f) / screenDimensions;

	float2 posScreen;
	if (viewport == 0)
	{
		posScreen = (float2(globalCord.xy) + 0.5f ) * screenPixelOffset.xy + float2(-1.0f,1.0f);
	}
	else if (viewport == 1)
	{
		posScreen = (float2(globalCord.x - screenDimensions.x, globalCord.y) + 0.5f ) * screenPixelOffset.xy + float2(-1.0f,1.0f);
	}
	else if (viewport == 2)
	{
		posScreen = (float2(globalCord.x, globalCord.y  - screenDimensions.y) + 0.5f ) * screenPixelOffset.xy + float2(-1.0f,1.0f);
	}
	else if (viewport == 3)
	{
		posScreen = (float2(globalCord.x - screenDimensions.x, globalCord.y  - screenDimensions.y) + 0.5f ) * screenPixelOffset.xy + float2(-1.0f,1.0f);
	}
	
	
	depth = Projection[viewport]._43 / (depth - Projection[viewport]._33);

	output.positionView = ReconstructPosViewFromDepth(posScreen, depth, viewport);
	output.normalView = mul(float4(normal,0), View[viewport]).xyz;
	
	return output;
}

void CalculateDepth(uint groupIndex, PixelData data)
{
	//gather info
	float minZ = camNearFar.y; //camNearFar.y
	float maxZ = camNearFar.x; //camNearFar.x
	
	float depth = data.positionView.z; 

	bool validPixel = depth >= camNearFar.x && 	//camNearFar.y
						depth < camNearFar.y;	//camNearFar.x

	if(validPixel)
	{
		minZ = min(minZ, depth); 
		maxZ = max(maxZ, depth);
	}
	

	if(groupIndex == 0)
	{
		minDepth = 0x7F7FFFFF;
		maxDepth = 0;
		visibleLightCount = 0;
	}

	GroupMemoryBarrierWithGroupSync();

	if(maxZ >= minZ)
	{
		InterlockedMax(maxDepth, asuint(maxZ));
		InterlockedMin(minDepth, asuint(minZ));
	}
	GroupMemoryBarrierWithGroupSync();
}

void CalculateFrustrums(uint2 groupID, inout float4 frustrumPlanes[6], int viewport)
{
	
	float minTileY = asfloat(minDepth); //
	float maxTileY = asfloat(maxDepth); //

	float2 viewportGroupID;
	if (viewport == 0)
	{
		viewportGroupID = groupID;
	}
	else if (viewport == 1)
	{
		viewportGroupID = float2( groupID.x - 60 , groupID.y);
	}
	else if (viewport == 2)
	{
		viewportGroupID = float2( groupID.x, groupID.y - 34);
	}
	else if (viewport == 3)
	{
		viewportGroupID = float2( groupID.x - 60, groupID.y - 34);
	}


	//										rcp =  1 / x in a fast way, could change to 1/float(2*BLOCK_SIZE)
	float2 tileScale = float2(screenDimensions.xy * rcp(float(2*BLOCK_SIZE)));
	float2 tileBias = tileScale - float2(viewportGroupID);

	//magic, getting the relevant columns from view matrix
	float4 c1 = float4(Projection[viewport]._11 * tileScale.x		, 0.0f											, tileBias.x	, 0.0f);
	float4 c2 = float4(0.0f											, -Projection[viewport]._22 * tileScale.y		, tileBias.y	, 0.0f);
	float4 c4 = float4(0.0f											,0.0f											,1.0f			, 0.0f);

	

	frustrumPlanes[0] = c4 - c1;
	frustrumPlanes[1] = c4 + c1;
	frustrumPlanes[2] = c4 - c2;
	frustrumPlanes[3] = c4 + c2;

	frustrumPlanes[4] = float4(0.0f, 0.0f, 1.0f, -minTileY);
	frustrumPlanes[5] = float4(0.0f, 0.0f, 1.0f, maxTileY);

	for (int i = 0; i < 4; i++) //normalize the planes
	{
		frustrumPlanes[i] *= rcp(length(frustrumPlanes[i].xyz));
	}
}



[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void CS( uint3 threadID		: SV_DispatchThreadID, 
		uint3 groupThreadID : SV_GroupThreadID,
		uint3 groupID		: SV_GroupID)
{
	uint groupIndex = groupThreadID.y * BLOCK_SIZE + groupThreadID.x;

	//get number of Lights
	uint maxNumOfLights, stride;
	lights.GetDimensions(maxNumOfLights, stride);

	
	int viewport;
	if (screenDimensions.x > threadID.x)
	{
		if (screenDimensions.y > threadID.y)
		{
			viewport = 0;
		}
		else
		{
			viewport = 2;
		}
	}
	else
	{
		if (screenDimensions.y > threadID.y)
		{
			viewport = 1;
		}
		else
		{
			viewport = 3;
		}
	}






	////////////////////////////
	/////////Get Data///////////
	////////////////////////////

	uint2 globalCord = threadID.xy;
	PixelData data = GetPixelData(globalCord, viewport);










	////////////////////////////
	/////////Calc Depth/////////
	////////////////////////////
	
	CalculateDepth(groupIndex ,data);











	////////////////////////////
	/////////Calc Frustum///////
	////////////////////////////

	float4 frustrumPlanes[6];
	CalculateFrustrums(groupID, frustrumPlanes, viewport);














	////////////////////////////
	/////////Calc lights////////
	////////////////////////////
	uint groupSize = BLOCK_SIZE * BLOCK_SIZE;
	//lightcull for this tile, may change more towards the dice implementation
	for (uint lightIndex = groupIndex; lightIndex < maxNumOfLights; lightIndex+= groupSize)
	{
		Light light = lights[lightIndex];


		bool inFrustum = true;
		for (int i = 0; i < 6 && inFrustum; i++)
		{
			float4 lightPos = mul(float4(light.position,1), View[viewport]);

			float distance = dot(frustrumPlanes[i], lightPos);
			inFrustum = inFrustum && (distance >= -light.radius);
		}
		if(inFrustum)
		{
			uint index;
			InterlockedAdd(visibleLightCount, 1, index); //outputs pre number to index
			visibleLightIndices[index] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();








	////////////////////////////
	/////////Add lights/////////
	////////////////////////////
	uint numOfLights = visibleLightCount;

	float3 finalColor = DiffuseColor_Spec[threadID.xy].xyz*0.5;
	float inSpec = DiffuseColor_Spec[threadID.xy].z/255;

	//if(all(globalCord < screenDimensions)) //checks for all components if blow zero, uses this for checking if outside screendim
	//{
		for (uint i = 0; i < visibleLightCount; i++)
		{
			uint lightIndex = visibleLightIndices[i];
			Light light = lights[lightIndex];
	
			finalColor += DirectIllumination(data.positionView, data.normalView , light, inSpec, viewport);
		}
	//}

	

	finalColor += Specular[threadID.xy].xyz;
	output[threadID.xy] = float4(finalColor.x,finalColor.y,finalColor.z, 1);
	


	/*float4 lightPos = mul(float4(lights[1].position,1), View[viewport]);

	float distance = dot(frustrumPlanes[0], lightPos);
	
	output[threadID.xy] = float4(distance,distance,distance, 1);*/

	
	//if (viewport == 0 )
	//{
	//	output[threadID.xy] = float4(1,0,0,0);
	//}
	//else if (viewport == 1)
	//{
	//	output[threadID.xy] = float4(1,1,0,0);
	//}
	//else if (viewport == 2)
	//{
	//	output[threadID.xy] = float4(0,0,1,0);

	//}
	//else if (viewport == 3)
	//{
	//	output[threadID.xy] = float4(0,1,0,0);
	//}
	/*if(Specular[globalCord].x == 1) 
	{
		output[threadID.xy] = float4(finalColor.x,finalColor.y,finalColor.z, 1);
	}*/
}


/*
http://software.intel.com/en-us/articles/microsoft-directcompute-on-intel-ivy-bridge-processor-graphics
http://stackoverflow.com/questions/12730309/rendering-to-multiple-textures-with-one-pass-in-directx-11
http://hieroglyph3.codeplex.com/
http://www.cse.chalmers.se/~uffe/tiled_shading_preprint.pdf


//newly added
http://www.gamedev.net/topic/474166-reconstructing-pixel-3d-position-from-depth/
http://www.gamedev.net/topic/506573-reconstructing-position-from-depth-data/


//super added!
http://www.gamedev.net/topic/648887-tile-frustum-construction-issue-for-tile-based-deferred-shading/
http://www.gamedev.net/topic/654360-tiled-deferred-frustum-culling-issues/
http://gamedev.stackexchange.com/questions/67431/deferred-tiled-shading-tile-frusta-calculation-in-opengl
http://www.opengpu.org/forum.php?mod=viewthread&tid=15995

*/