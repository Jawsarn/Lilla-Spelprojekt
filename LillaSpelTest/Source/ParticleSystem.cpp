#include "ParticleSystem.h"
#include <iostream>

ParticleSystem* ParticleSystem::singleton = nullptr;

ParticleSystem* ParticleSystem::GetInstance()
{
	if(singleton == nullptr)
	{
		singleton = new ParticleSystem();
	}
	return singleton;
}

ParticleSystem::ParticleSystem()
{

}


ParticleSystem::~ParticleSystem(void)
{

}

HRESULT ParticleSystem::Initialize( ID3D11Device* p_Device, ID3D11DeviceContext* p_DeviceContext, ID3D11DepthStencilState* p_NoWriteDepthState, ID3D11DepthStencilState* p_DepthOff, ID3D11BlendState* p_BlendOn,ID3D11BlendState* p_BlendOff, ID3D11Buffer* p_PerFrameBuffer , ID3D11Buffer* p_PerObjectBuffer)
{
	HRESULT hr = S_OK;
	m_ShaderLoader = new ShaderLoader();
	m_Device = p_Device;
	m_DeviceContext = p_DeviceContext;
	m_PerFrameBuffer = p_PerFrameBuffer;
	m_PerObjectBuffer = p_PerObjectBuffer;


	m_NoWriteDepthState = p_NoWriteDepthState;
	m_DepthOff = p_DepthOff;
	m_BlendOn = p_BlendOn;
	m_BlendOff = p_BlendOff;


	hr = CreateConstantBuffer();
	if( FAILED( hr ) )
        return hr;

	hr = CreateRandomTexture1DSRV();
	if( FAILED( hr ) )
		return hr;

	hr = CreateShaders();


	return hr;
}

HRESULT ParticleSystem::CreateShaders()
{
	HRESULT hr = S_OK;

	//Create the particle update Vertex shader
	D3D11_INPUT_ELEMENT_DESC t_Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "LIFESPAN", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};


	ID3D11VertexShader* t_NewVertShader;
	ID3D11InputLayout* t_NewInputLayout;
	UINT t_NumElements = ARRAYSIZE( t_Layout );
	hr = m_ShaderLoader->CreateVertexShaderWithInputLayout( L"GraphicParticleUpdateVS.hlsl", "VS", "vs_5_0",m_Device, &t_NewVertShader, t_Layout, t_NumElements, &t_NewInputLayout);
	if( FAILED( hr ) )
        return hr;

	m_VertexShaders.push_back(t_NewVertShader);
	m_InputLayouts.push_back(t_NewInputLayout);
	

	//CREATE SHADER 2
	hr = m_ShaderLoader->CreateVertexShaderWithInputLayout(L"GraphicParticleDrawVS.hlsl", "VS", "vs_5_0",m_Device, &t_NewVertShader, t_Layout, t_NumElements, &t_NewInputLayout);
	if( FAILED( hr ) )
		return hr;

	m_VertexShaders.push_back( t_NewVertShader );
	m_InputLayouts.push_back( t_NewInputLayout );


	//geometry shaders

	ID3D11GeometryShader* t_NewGeoShader;
	hr = m_ShaderLoader->CreateGeometryShader(L"GraphicParticleDrawGS.hlsl", "GS", "gs_5_0", m_Device, &t_NewGeoShader);
    if( FAILED( hr ) )
        return hr;

	m_GeometryShaders.push_back(t_NewGeoShader);

	D3D11_SO_DECLARATION_ENTRY t_Decl[] =
	{
		// semantic name, semantic index, start component, component count, output slot
		{ 0 ,"POSITION", 0, 0, 3, 0 },   // output all components of position
		{ 0 ,"VELOCITY", 0, 0, 3, 0 },     // output the first 3 of the normal
		{ 0 ,"SIZE",	 0, 0, 2, 0 },
		{ 0 ,"AGE",		 0, 0, 1, 0 },
		{ 0 ,"LIFESPAN", 0, 0, 1, 0 },
		{ 0 ,"TYPE",	 0, 0, 1, 0 },// output the first 2 texture coordinates
	};

	t_NumElements = ARRAYSIZE(t_Decl);

	hr = m_ShaderLoader->CreateGeometryShaderWithOutputDeclaration( L"GraphicParticleUpdateGS.hlsl", "GS", "gs_5_0", m_Device, &t_NewGeoShader, t_Decl, t_NumElements);
	if( FAILED( hr ) )
		return hr;


	m_GeometryShaders.push_back(t_NewGeoShader);


	//create pixel shaders

	ID3D11PixelShader* t_NewPixShader;

	hr = m_ShaderLoader->CreatePixelShader(L"GraphicParticleDrawPS.hlsl","PS","ps_5_0",m_Device, &t_NewPixShader);
	if( FAILED( hr ) )
		return hr;

	m_PixelShaders.push_back(t_NewPixShader);

	ParticleShaderProgram t_NewParticleShaderProgram;
	t_NewParticleShaderProgram.updateVertexShader = m_VertexShaders.size() - 2;
	t_NewParticleShaderProgram.drawVertexShader = m_VertexShaders.size() - 1;
	t_NewParticleShaderProgram.updateGeometryShader = m_GeometryShaders.size() - 1;
	t_NewParticleShaderProgram.drawGeometryShader = m_GeometryShaders.size() - 2;
	t_NewParticleShaderProgram.updateInputLayout = m_InputLayouts.size() - 1;
	t_NewParticleShaderProgram.drawInputLayout = m_InputLayouts.size() - 2;

	t_NewParticleShaderProgram.drawPixelShader = m_PixelShaders.size() - 1;

	m_ParticleShaderPrograms.push_back(t_NewParticleShaderProgram);

	return hr;
}

HRESULT ParticleSystem::CreateParticleSystem(UINT p_EffectType, const wchar_t * p_FileName , UINT p_StartBufferID, XMFLOAT3 p_ObjectPosition, UINT p_DataID, UINT p_MaxParticles, XMFLOAT3 p_Color,UINT &systemID)
{
	HRESULT hr = S_OK;

	ParticleEffectSystem t_NewSystem;

	t_NewSystem.programID = p_EffectType;

	ID3D11ShaderResourceView* t_NewTexture;

	hr = CreateDDSTextureFromFile(m_Device, p_FileName,nullptr, &t_NewTexture);
	m_TextureViews.push_back(t_NewTexture);

	t_NewSystem.textureID = m_TextureViews.size() -1;
	t_NewSystem.startBufferID = p_StartBufferID;
	t_NewSystem.objectPosition = p_ObjectPosition; 
	t_NewSystem.perEffectDataID = p_DataID;
	t_NewSystem.color = p_Color;

	t_NewSystem.firstrun = true;


	D3D11_BUFFER_DESC t_BufferDesc;
	t_BufferDesc.ByteWidth = sizeof(Particle)*p_MaxParticles;
	t_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	t_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	t_BufferDesc.CPUAccessFlags = 0;
	t_BufferDesc.MiscFlags = 0;
	t_BufferDesc.StructureByteStride = 0;
	
	ID3D11Buffer* t_NewBuff;

	hr = m_Device->CreateBuffer( &t_BufferDesc, NULL, &t_NewBuff );
	if( FAILED( hr) )
		return hr;

	m_ParticleVertexBuffer.push_back(t_NewBuff);

	m_Device->CreateBuffer( &t_BufferDesc, NULL, &t_NewBuff );
	if( FAILED( hr) )
		return hr;

	m_ParticleVertexBuffer.push_back(t_NewBuff);

	t_NewSystem.drawVertexBufferID = (m_VertexShaders.size() - 1); //could've put this as a general thing that it was + 1, but for reading purposes I put it as a variable
	t_NewSystem.updateVertexBufferID = (m_VertexShaders.size() -2);


	m_ParticleEffectSystems.push_back(t_NewSystem);
	systemID = m_ParticleEffectSystems.size() - 1;

	return hr;
}

HRESULT ParticleSystem::CreateInitParticlesBuffer(std::vector<Particle> p_StartParticles, UINT &o_BufferID)
{
	HRESULT hr = S_OK;

	UINT t_NumOfElements = p_StartParticles.size();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * t_NumOfElements;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	ZeroMemory(&vinitData,sizeof(vinitData));
	vinitData.pSysMem = &p_StartParticles[0];

	ID3D11Buffer* t_NewBuff; 
	hr = (m_Device->CreateBuffer(&vbd, &vinitData, &t_NewBuff));

	VertexBufferWithNumOfVert t_NewVwNum;
	t_NewVwNum.vertexBuffer = t_NewBuff;
	t_NewVwNum.numOfVertices = t_NumOfElements;

	m_InitParticleVertexBuffersWithNum.push_back(t_NewVwNum);

	o_BufferID = m_InitParticleVertexBuffersWithNum.size() - 1;

	return hr;
}

void ParticleSystem::CreateCBsetup(XMFLOAT3 p_SpawnPosition, XMFLOAT3 p_EmitDirection, float p_InitSpawnAmount, float p_ParticleLifeSpan, XMFLOAT2 p_InitialSize, float p_SpawnTime, UINT &o_DataID)
{
	CPerEffectBuffer t_NewCBSetup;
	t_NewCBSetup.spawnPosition = p_SpawnPosition;
	t_NewCBSetup.emitDirection = p_EmitDirection;
	t_NewCBSetup.initSpawnAmount = p_InitSpawnAmount;
	t_NewCBSetup.particleLifeSpan = p_ParticleLifeSpan;
	t_NewCBSetup.initialSize = p_InitialSize;
	t_NewCBSetup.spawnTime = p_SpawnTime;

	m_PerEffectData.push_back(t_NewCBSetup);

	o_DataID = m_PerEffectData.size() -1;
}

HRESULT ParticleSystem::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CPerEffectBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
	hr = m_Device->CreateBuffer( &bd, nullptr, &m_PerEffectBuffer );
	if( FAILED(hr) )
		return hr;
							  

	return hr;
}

HRESULT ParticleSystem::CreateRandomTexture1DSRV()
{
	HRESULT hr = S_OK;

	//
	// Create the random data.
	XMFLOAT4 randVal[512];

	for(int i = 0; i < 512; ++i)
	{
		randVal[i].x = RandF(-1.0f, 1.0f);
		randVal[i].y = RandF(-1.0f, 1.0f);
		randVal[i].z = RandF(-1.0f, 1.0f);
		randVal[i].w = RandF(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randVal;
	initData.SysMemPitch = 512*sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;
	
	// Create the texture.
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 512;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;


	ID3D11Texture1D* randomTex = 0;
	hr = m_Device->CreateTexture1D(&texDesc, &initData, &randomTex);
	if( FAILED(hr) )
		return hr;
	
	// Create the resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	hr = m_Device->CreateShaderResourceView(randomTex, &viewDesc, &m_RandomTexSRV);
	if( FAILED(hr) )
		return hr;

	m_DeviceContext->GSSetShaderResources(0,1,&m_RandomTexSRV);

	return hr;
}

HRESULT ParticleSystem::UpdatePositionOnCBsetup(UINT p_ParticleSystemID, CXMMATRIX p_WorldMatrix)
{
	HRESULT hr = S_OK;

	XMVECTOR t_PosVector = XMLoadFloat3(&m_ParticleEffectSystems[p_ParticleSystemID].objectPosition);
	
	t_PosVector = XMVector3Transform(t_PosVector, p_WorldMatrix);

	XMStoreFloat3( &m_PerEffectData[m_ParticleEffectSystems[p_ParticleSystemID].perEffectDataID].spawnPosition, t_PosVector);

	return hr;
}

float ParticleSystem::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}
// Returns random float in [a, b).
float ParticleSystem::RandF(float a, float b)
{
	return a + RandF()*(b-a);
}
XMVECTOR ParticleSystem::RandUnitVec3()
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	//XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(
			RandF(-1.0f, 1.0f),
			RandF(-1.0f, 1.0f),
			RandF(-1.0f, 1.0f), 0.0f);
		// Ignore points outside the unit sphere in order to
		// get an even distribution over the unit sphere. Otherwise
		// points will clump more on the sphere near the corners
		// of the cube.
		if(XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		return XMVector3Normalize(v);
	}
}


void ParticleSystem::Draw(float dt)
{
	// make the OM to the backbuffer RENDER TARGET MAKE FIX OMG LULZ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NOT FIXED YET LULZ
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	m_DeviceContext->HSSetShader(nullptr,nullptr,0);
	m_DeviceContext->DSSetShader(nullptr,nullptr,0);




	//old way
	for (int i = 0; i < m_ParticleEffectSystems.size(); i++)
	{
		ParticleEffectSystem t_CurSys = m_ParticleEffectSystems[i];
		
		PerObjectBuffer t_pobj;
		t_pobj.Color = t_CurSys.color;
		t_pobj.typeOfObject = 0;
		t_pobj.World = XMMatrixIdentity();

		m_DeviceContext->UpdateSubresource( m_PerObjectBuffer, 0, nullptr, &t_pobj, 0, 0 );

		//update effect buffer data
		m_PerEffectData[t_CurSys.perEffectDataID].deltaTime = dt;
		m_DeviceContext->UpdateSubresource( m_PerEffectBuffer, 0, nullptr, &m_PerEffectData[t_CurSys.perEffectDataID], 0, 0 );

		//turn off depth 
		m_DeviceContext->OMSetDepthStencilState(m_DepthOff, 0);

		//turn blend on
		float t_BlendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
		m_DeviceContext->OMSetBlendState(m_BlendOn,t_BlendFactors,0xffffffff);

		//update the particles
		UpdateParticles(i);

		//now swap buffers
		ID3D11Buffer* bufferArray[1] = {0};
		UINT offset = 0;

		m_DeviceContext->SOSetTargets(1,bufferArray,&offset);
		//swap buffers
		std::swap( m_ParticleVertexBuffer[t_CurSys.drawVertexBufferID],m_ParticleVertexBuffer[t_CurSys.updateVertexBufferID]);

		//turn on depth with depth check but no write  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NOT FIXED YET
		m_DeviceContext->OMSetDepthStencilState(m_NoWriteDepthState,0);

		DrawParticles(i);

		//turn blend off
		m_DeviceContext->OMSetBlendState(m_BlendOff, t_BlendFactors,0xffffffff);
	}
}

void ParticleSystem::UpdateParticles(UINT id)
{
	//sset the right buffers
	m_DeviceContext->GSSetConstantBuffers(0, 1, &m_PerEffectBuffer );

	
	ParticleShaderProgram t_CurSysProgram = m_ParticleShaderPrograms[m_ParticleEffectSystems[id].programID];
	
	//set shaders
	m_DeviceContext->VSSetShader(m_VertexShaders[t_CurSysProgram.updateVertexShader],nullptr,0);
	m_DeviceContext->GSSetShader(m_GeometryShaders[t_CurSysProgram.updateGeometryShader],nullptr,0);
	m_DeviceContext->PSSetShader(nullptr,nullptr,0);
	
	//set input layout
	m_DeviceContext->IASetInputLayout(m_InputLayouts[t_CurSysProgram.updateInputLayout]);

	UINT stride = sizeof( Particle );
	UINT offset = 0;

	//set the input vertex buffer
	if(m_ParticleEffectSystems[id].firstrun)
	{
		m_DeviceContext->IASetVertexBuffers( 0, 1, &m_InitParticleVertexBuffersWithNum[m_ParticleEffectSystems[id].startBufferID].vertexBuffer, &stride, &offset );
	}
	else
	{
		m_DeviceContext->IASetVertexBuffers( 0, 1, &m_ParticleVertexBuffer[m_ParticleEffectSystems[id].drawVertexBufferID], &stride, &offset );
	}
	
	//set the output vertex buffer
	m_DeviceContext->SOSetTargets(1, &m_ParticleVertexBuffer[m_ParticleEffectSystems[id].updateVertexBufferID], &offset);

	//make the draw call based on firstrun
	if(m_ParticleEffectSystems[id].firstrun)
	{
		m_DeviceContext->Draw(m_InitParticleVertexBuffersWithNum[m_ParticleEffectSystems[id].startBufferID].numOfVertices, 0);
		m_ParticleEffectSystems[id].firstrun = false;
	}
	else
	{
		m_DeviceContext->DrawAuto();
	}
}

void ParticleSystem::DrawParticles(UINT id)
{
	//set buffers right
	m_DeviceContext->GSSetConstantBuffers(0, 1, &m_PerFrameBuffer);
	
	ParticleShaderProgram t_CurSysProgram = m_ParticleShaderPrograms[m_ParticleEffectSystems[id].programID];
	
	//Set shaders 
	m_DeviceContext->VSSetShader(m_VertexShaders[t_CurSysProgram.drawVertexShader],nullptr,0);
	m_DeviceContext->GSSetShader(m_GeometryShaders[t_CurSysProgram.drawGeometryShader],nullptr,0);
	m_DeviceContext->PSSetShader(m_PixelShaders[t_CurSysProgram.drawPixelShader],nullptr,0);

	//set input layout
	m_DeviceContext->IASetInputLayout(m_InputLayouts[m_ParticleShaderPrograms[m_ParticleEffectSystems[id].programID].drawInputLayout]);

	UINT stride = sizeof( Particle );
	UINT offset = 0;

	//set input vertex buffer
	m_DeviceContext->IASetVertexBuffers( 0, 1, &m_ParticleVertexBuffer[m_ParticleEffectSystems[id].drawVertexBufferID], &stride, &offset);
	
	//update texture
	m_DeviceContext->PSSetShaderResources(0, 1, &m_TextureViews[m_ParticleEffectSystems[id].textureID]);

	//draw the particles form memory
	m_DeviceContext->DrawAuto();
}

//void ParticleSystem::Cleanup()
//{
//	if( perEffectBuffer ) perEffectBuffer->Release();
//	if( perFrameBuffer ) perFrameBuffer->Release();
//	if( randomTexSRV )randomTexSRV->Release();
//}