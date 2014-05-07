//#include "ParticleSystem.h"
//#include <iostream>
//
//ParticleSystem* ParticleSystem::singleton = nullptr;
//
//ParticleSystem* ParticleSystem::GetInstance()
//{
//	if(singleton == nullptr)
//	{
//		singleton = new ParticleSystem();
//	}
//	return singleton;
//}
//
//ParticleSystem::ParticleSystem()
//{
//	perEffectBuffer = nullptr;
//	perFrameBuffer = nullptr;
//	firstRun = true;
//}
//
//
//ParticleSystem::~ParticleSystem(void)
//{
//	Cleanup();
//}
//
//HRESULT ParticleSystem::Initialize( ID3D11Device* inDevice, ID3D11DeviceContext* inDeviceContext, ID3D11DepthStencilState* onDepthState, ID3D11DepthStencilState* offDepthState, ID3D11BlendState* onBlendState, ID3D11BlendState* offBlendState, ID3D11Buffer* perObjBuff)
//{
//	HRESULT hr = S_OK;
//	shaderLoader = new ShaderLoader();
//	device = inDevice;
//	deviceContext = inDeviceContext;
//	depthOn = onDepthState;
//	depthOff = offDepthState;
//	blendOn = onBlendState;
//	blendOff = offBlendState;
//	perObjBuffer = perObjBuff;
//
//	hr = CreateConstantBuffer();
//	if( FAILED( hr ) )
//        return hr;
//
//	hr = CreateRandomTexture1DSRV();
//	if( FAILED( hr ) )
//        return hr;
//
//	hr = CreateShaders();
//
//
//	return hr;
//}
//
//HRESULT ParticleSystem::CreateShaders()
//{
//	HRESULT hr = S_OK;
//	
//    D3D11_INPUT_ELEMENT_DESC layout[] =
//    {
//        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "LIFESPAN", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		
//	};
//
//
//	ID3D11VertexShader* newVertShader;
//	ID3D11InputLayout* newInputLayout;
//	UINT numElements = ARRAYSIZE( layout );
//	hr = shaderLoader->CreateVertexShaderWithInputLayout( L"UpdateVertexShader.hlsl", "VS", "vs_5_0",device,&newVertShader,layout,numElements,&newInputLayout);
//	if( FAILED( hr ) )
//        return hr;
//
//	vertexShaders.push_back(newVertShader);
//	inputLayouts.push_back(newInputLayout);
//	
//
//	//CREATE SHADER 2
//	hr = shaderLoader->CreateVertexShaderWithInputLayout(L"DrawVertexShader.hlsl", "VS", "vs_5_0",device,&newVertShader,layout,numElements,&newInputLayout);
//	if( FAILED( hr ) )
//        return hr;
//
//	vertexShaders.push_back(newVertShader);
//	inputLayouts.push_back(newInputLayout);
//
//
//	//////geometry shaders
//
//	ID3D11GeometryShader* newGeoShader;
//	hr = shaderLoader->CreateGeometryShader(L"DrawGeometryShader.hlsl", "GS", "gs_5_0", device, &newGeoShader);
//    if( FAILED( hr ) )
//        return hr;
//
//	geometryShaders.push_back(newGeoShader);
//
//	D3D11_SO_DECLARATION_ENTRY pDecl[] =
//	{
//		// semantic name, semantic index, start component, component count, output slot
//		{ 0 ,"POSITION", 0, 0, 3, 0 },   // output all components of position
//		{ 0 ,"VELOCITY", 0, 0, 3, 0 },     // output the first 3 of the normal
//		{ 0 ,"SIZE",	 0, 0, 2, 0 },
//		{ 0 ,"AGE",		 0, 0, 1, 0 },
//		{ 0 ,"LIFESPAN", 0, 0, 1, 0 },
//		{ 0 ,"TYPE",	 0, 0, 1, 0 },// output the first 2 texture coordinates
//	};
//
//	numElements = ARRAYSIZE(pDecl);
//
//	hr = shaderLoader->CreateGeometryShaderWithOutputDeclaration( L"UpdateGeometryShader.hlsl", "GS", "gs_5_0",device,&newGeoShader,pDecl,numElements);
//	
//	geometryShaders.push_back(newGeoShader);
//
//
//	//create pixel shaders
//
//	ID3D11PixelShader* newPixShader;
//
//	hr = shaderLoader->CreatePixelShader(L"DrawPixelShader.hlsl","PS","ps_5_0",device,&newPixShader);
//
//	pixelShaders.push_back(newPixShader);
//
//	ParticleShaderProgram psp;
//	psp.updateVertexShader = 0;
//	psp.drawVertexShader = 1;
//	psp.updateGeometryShader = 1;
//	psp.drawGeometryShader = 0;
//	psp.updateInputLayout = 0;
//	psp.drawInputLayout = 1;
//
//	psp.drawPixelShader = 0;
//
//	particleShaderPrograms.push_back(psp);
//
//	return hr;
//}
//
//HRESULT ParticleSystem::LoadTexture(const wchar_t * fileName, UINT &outID)
//{
//	HRESULT hr = S_OK;
//	ID3D11ShaderResourceView* newSRV;
//	hr = CreateDDSTextureFromFile(device, fileName, nullptr, &newSRV);
//	if( FAILED( hr ) )
//		return hr;
//
//	textureViews.push_back(newSRV);
//	outID = textureViews.size() - 1;
//
//	return hr;
//}
//
//HRESULT ParticleSystem::CreateParticleSystem(UINT type, UINT textureID , UINT startBufferID,CXMMATRIX world,UINT dataID, UINT maxParticles,UINT &systemID)
//{
//	HRESULT hr = S_OK;
//
//	ParticleEffectSystem newSys;
//
//	newSys.programID = type;
//	newSys.textureID = textureID;
//	newSys.startBufferID = startBufferID;
//	newSys.world = world;
//	newSys.perEffectDataID = dataID;
//	newSys.firstrun = true;
//
//	
//
//
//	D3D11_BUFFER_DESC bdsc;
//	bdsc.ByteWidth = sizeof(Particle)*maxParticles;
//	bdsc.Usage = D3D11_USAGE_DEFAULT;
//	bdsc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
//	bdsc.CPUAccessFlags = 0;
//	bdsc.MiscFlags = 0;
//	bdsc.StructureByteStride = 0;
//	
//	ID3D11Buffer* newBuff;
//
//	hr = device->CreateBuffer( &bdsc, NULL, &newBuff );
//	if( FAILED( hr) )
//		return hr;
//
//	particleVertexBuffer.push_back(newBuff);
//
//	device->CreateBuffer( &bdsc, NULL, &newBuff );
//	if( FAILED( hr) )
//		return hr;
//
//	particleVertexBuffer.push_back(newBuff);
//
//	newSys.drawVertexBufferID = (vertexShaders.size() - 1); //could've put this as a general thing that it was + 1, but for reading purposes I put it as a variable
//	newSys.updateVertexBufferID = (vertexShaders.size() -2);
//
//
//	particleEffectSystems.push_back(newSys);
//	systemID = particleEffectSystems.size() - 1;
//
//	return hr;
//}
//
//HRESULT ParticleSystem::CreateStartParticlesBuffer(std::vector<Particle> startParticles, UINT &bufferID)
//{
//	HRESULT hr = S_OK;
//
//	UINT numOfElements = startParticles.size();
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_DEFAULT;
//	vbd.ByteWidth = sizeof(Particle) * numOfElements;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//	vbd.MiscFlags = 0;
//	vbd.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA vinitData;
//	ZeroMemory(&vinitData,sizeof(vinitData));
//	vinitData.pSysMem = &startParticles[0];
//
//	ID3D11Buffer* newBuff; 
//	hr = (device->CreateBuffer(&vbd, &vinitData, &newBuff));
//	
//	VertexBufferWithNumOfVert newVwNum;
//	newVwNum.vertexBuffer = newBuff;
//	newVwNum.numOfVertices = numOfElements;
//
//	initVertexBuffersWithNum.push_back(newVwNum);
//
//	bufferID = initVertexBuffersWithNum.size() - 1;
//
//	return hr;
//}
//
//void ParticleSystem::CreateCBsetup(XMFLOAT3 worldAcceler, float flareEmitNumber, XMFLOAT3 emitDirection, float initSpawnAmount, float particleLifeSpan, XMFLOAT2 initialSize, UINT &dataID)
//{
//	CPerEffectBuffer newBuf;
//	newBuf.worldAcceler = worldAcceler;
//	newBuf.flareEmitNumber = flareEmitNumber;
//	newBuf.emitDirection = emitDirection;
//	newBuf.initSpawnAmount = initSpawnAmount;
//	newBuf.particleLifeSpan = particleLifeSpan;
//	newBuf.initialSize = initialSize;
//	newBuf.filler = 0;
//	
//	perEffectData.push_back(newBuf);
//
//	dataID = perEffectData.size() -1;
//}
//
//HRESULT ParticleSystem::CreateConstantBuffer()
//{
//	HRESULT hr = S_OK;
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory( &bd, sizeof(bd) );
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(CPerEffectBuffer);
//    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    bd.CPUAccessFlags = 0;
//	hr = device->CreateBuffer( &bd, nullptr, &perEffectBuffer );
//	if( FAILED(hr) )
//		return hr;
//
//	bd.ByteWidth = sizeof(CPerFrameParticleBuffer);
//
//	hr = device->CreateBuffer( &bd, nullptr, &perFrameBuffer); 
//
//	deviceContext->VSSetConstantBuffers(4,1,&perEffectBuffer);
//	deviceContext->VSSetConstantBuffers(5,1,&perFrameBuffer);
//
//	deviceContext->GSSetConstantBuffers(4,1,&perEffectBuffer);
//	deviceContext->GSSetConstantBuffers(5,1,&perFrameBuffer);
//
//	return hr;
//}
//
//HRESULT ParticleSystem::CreateRandomTexture1DSRV()
//{
//	HRESULT hr = S_OK;
//
//	//
//	// Create the random data.
//	XMFLOAT4 randVal[1024];
//
//	for(int i = 0; i < 1024; ++i)
//	{
//		randVal[i].x = MathHelper::RandF(-1.0f, 1.0f);
//		randVal[i].y = MathHelper::RandF(-1.0f, 1.0f);
//		randVal[i].z = MathHelper::RandF(-1.0f, 1.0f);
//		randVal[i].w = MathHelper::RandF(-1.0f, 1.0f);
//	}
//
//	D3D11_SUBRESOURCE_DATA initData;
//	initData.pSysMem = randVal;
//	initData.SysMemPitch = 1024*sizeof(XMFLOAT4);
//	initData.SysMemSlicePitch = 0;
//	
//	// Create the texture.
//	D3D11_TEXTURE1D_DESC texDesc;
//	texDesc.Width = 1024;
//	texDesc.MipLevels = 1;
//	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	texDesc.CPUAccessFlags = 0;
//	texDesc.MiscFlags = 0;
//	texDesc.ArraySize = 1;
//
//
//	ID3D11Texture1D* randomTex = 0;
//	hr = device->CreateTexture1D(&texDesc, &initData, &randomTex);
//	if( FAILED(hr) )
//		return hr;
//	
//	// Create the resource view.
//	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
//	viewDesc.Format = texDesc.Format;
//	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
//	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
//	viewDesc.Texture1D.MostDetailedMip = 0;
//
//	hr = device->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV);
//	if( FAILED(hr) )
//		return hr;
//
//	deviceContext->GSSetShaderResources(9,1,&randomTexSRV);
//
//	return hr;
//}
//
//void ParticleSystem::Draw(float dt, float gt)
//{
//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//
//	CPerFrameParticleBuffer cpfpb;
//	cpfpb.deltaTime = dt;
//	cpfpb.gametime = gt;
//
//	deviceContext->UpdateSubresource( perFrameBuffer, 0, nullptr, &cpfpb, 0, 0 );
//	deviceContext->HSSetShader(nullptr,nullptr,0);
//	deviceContext->DSSetShader(nullptr,nullptr,0);
//
//	for (int i = 0; i < particleEffectSystems.size(); i++)
//	{
//		ParticleEffectSystem curSys = particleEffectSystems[i];
//		
//
//		//update buffer
//		PerObjectBuffer pobj;
//		pobj.fillers = XMFLOAT3(0,0,0);
//		pobj.hasTexture = 1;
//		pobj.world = XMMatrixTranspose( curSys.world );
//		deviceContext->UpdateSubresource(perObjBuffer,0,nullptr,&pobj,0,0);
//		
//		deviceContext->UpdateSubresource( perEffectBuffer, 0, nullptr, &perEffectData[curSys.perEffectDataID], 0, 0 );
//
//		//turn off depth
//		deviceContext->OMSetDepthStencilState(depthOff,0);
//
//		//turn blend on
//		float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
//		deviceContext->OMSetBlendState(blendOn,blendFactors,0xffffffff);
//
//		UpdateParticles(i);
//
//		ID3D11Buffer* bufferArray[1] = {0};
//		UINT offset = 0;
//
//		deviceContext->SOSetTargets(1,bufferArray,&offset);
//		//swap buffers
//		std::swap( particleVertexBuffer[curSys.drawVertexBufferID],particleVertexBuffer[curSys.updateVertexBufferID]);
//
//		//turn on depth
//		deviceContext->OMSetDepthStencilState(depthOn,0);
//		
//
//		DrawParticles(i);
//
//		//turn blend off
//		float blendFactors2[] = {0.0f, 0.0f, 0.0f, 0.0f};
//		deviceContext->OMSetBlendState(blendOff,blendFactors2,0xffffffff);
//	}
//}
//
//void ParticleSystem::UpdateParticles(UINT id)
//{
//	ParticleShaderProgram sys = particleShaderPrograms[particleEffectSystems[id].programID];
//	deviceContext->VSSetShader(vertexShaders[sys.updateVertexShader],nullptr,0);
//	deviceContext->GSSetShader(geometryShaders[sys.updateGeometryShader],nullptr,0);
//	deviceContext->PSSetShader(nullptr,nullptr,0);
//	
//	deviceContext->IASetInputLayout(inputLayouts[sys.updateInputLayout]);
//
//	UINT stride = sizeof( Particle );
//	UINT offset = 0;
//
//	if(particleEffectSystems[id].firstrun)
//	{
//		deviceContext->IASetVertexBuffers( 0, 1, &initVertexBuffersWithNum[particleEffectSystems[id].startBufferID].vertexBuffer, &stride, &offset );
//	}
//	else
//	{
//		deviceContext->IASetVertexBuffers( 0, 1, &particleVertexBuffer[particleEffectSystems[id].drawVertexBufferID], &stride, &offset );
//	}
//
//	deviceContext->SOSetTargets(1, &particleVertexBuffer[particleEffectSystems[id].updateVertexBufferID], &offset);
//
//	if(particleEffectSystems[id].firstrun)
//	{
//		deviceContext->Draw(initVertexBuffersWithNum[particleEffectSystems[id].startBufferID].numOfVertices, 0);
//		particleEffectSystems[id].firstrun = false;
//	}
//	else
//	{
//		deviceContext->DrawAuto();
//	}
//}
//
//void ParticleSystem::DrawParticles(UINT id)
//{
//	ParticleShaderProgram sys = particleShaderPrograms[particleEffectSystems[id].programID];
//	deviceContext->VSSetShader(vertexShaders[sys.drawVertexShader],nullptr,0);
//	deviceContext->GSSetShader(geometryShaders[sys.drawGeometryShader],nullptr,0);
//	deviceContext->PSSetShader(pixelShaders[sys.drawPixelShader],nullptr,0);
//
//	deviceContext->IASetInputLayout(inputLayouts[particleShaderPrograms[particleEffectSystems[id].programID].drawInputLayout]);
//
//	UINT stride = sizeof( Particle );
//	UINT offset = 0;
//	deviceContext->IASetVertexBuffers( 0, 1, &particleVertexBuffer[particleEffectSystems[id].drawVertexBufferID], &stride, &offset);
//
//	deviceContext->DrawAuto();
//}
//
//void ParticleSystem::Cleanup()
//{
//	if( perEffectBuffer ) perEffectBuffer->Release();
//	if( perFrameBuffer ) perFrameBuffer->Release();
//	if( randomTexSRV )randomTexSRV->Release();
//}