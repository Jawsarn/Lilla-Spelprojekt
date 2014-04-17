#include "GraphicEngine.h"
#include "MeshLoader.h"
#include <exception>



GraphicEngine* GraphicEngine::singleton = nullptr;

GraphicEngine* GraphicEngine::GetInstance()
{
	if(singleton == nullptr)
	{
		singleton = new GraphicEngine();
	}
	return singleton;
}

GraphicEngine::GraphicEngine(void)
{
	m_DriverType = D3D_DRIVER_TYPE_NULL;
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	m_ShaderLoader = new ShaderLoader();
}

GraphicEngine::~GraphicEngine(void)
{
	
}

HRESULT GraphicEngine::Initialize( UINT p_Width, UINT p_Height, HWND handleWindow )
{
	HRESULT hr = S_OK;
	
	hr = InitializeDriverAndVersion(handleWindow);
	if( FAILED( hr ) )
		return hr;

	hr = InitializeRenderTargetView();
	if( FAILED( hr ) )
		return hr;

	SetViewports();

	hr = CreateRasterizers();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeDepthAndDepthStates();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeBlendAndBlendStates();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeShaders();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeConstantBuffers();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeGBuffers();
	if( FAILED( hr ) )
		return hr;

	hr = InitializeSamplerState();
	if( FAILED( hr) )
		return hr;

	/*particleSystem = particleSystem->GetInstance();
	hr = particleSystem->Initialize(device,deviceContext,depthStateOn,depthStateOff, blendStateOn, blendStateOff,perObjectBuffer);
	if( FAILED( hr ) )
		return hr;*/

	return hr;
}


HRESULT GraphicEngine::InitializeDriverAndVersion( HWND handleWindow)
{
	HRESULT hr = S_OK;
    
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_Width;
    sd.BufferDesc.Height = m_Height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS; //fixed for deferred rendering, set flag for input if want to put into a shader as well I guess
	sd.OutputWindow = handleWindow;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_DriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(
			nullptr, 
			m_DriverType, 
			nullptr, 
			createDeviceFlags, 
			featureLevels, 
			numFeatureLevels,
			D3D11_SDK_VERSION, 
			&sd, 
			&m_SwapChain, 
			&m_Device, 
			&m_FeatureLevel, 
			&m_DeviceContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDeviceAndSwapChain(
				nullptr, 
				m_DriverType, 
				nullptr, 
				createDeviceFlags, 
				&featureLevels[1], 
				numFeatureLevels - 1, 
				D3D11_SDK_VERSION, &sd,
				&m_SwapChain,
				&m_Device, 
				&m_FeatureLevel, 
				&m_DeviceContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Obtain the Direct3D 11.1 versions if available
    hr = m_Device->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast<void**>( &m_Device1 ) );
    if ( SUCCEEDED(hr) )
    {
		(void)m_DeviceContext->QueryInterface( __uuidof( ID3D11DeviceContext1 ), reinterpret_cast<void**>( &m_DeviceContext1 ) );
    }
	return hr;
}

HRESULT GraphicEngine::InitializeRenderTargetView()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* t_BackBuffer = nullptr;
    hr = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&t_BackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = m_Device->CreateRenderTargetView( t_BackBuffer, nullptr, &m_RenderTargetView );
    if( FAILED( hr ) )
		return hr;

	hr = m_Device->CreateUnorderedAccessView(t_BackBuffer, nullptr,&m_BackBufferUAV);
	if( FAILED( hr) )
		return hr;
	
	t_BackBuffer->Release();


	return hr;
}

void GraphicEngine::SetViewports()
{
	//when adding more fix
	/*FLOAT t_HalfWidth = (FLOAT)m_Width/2;
	FLOAT t_HalfHeight = (FLOAT)m_Height/2;

    D3D11_VIEWPORT vp[4];
	for (int i = 0; i < 4; i++)
	{
		vp[i].Width = t_HalfWidth;
		vp[i].Height = t_HalfHeight;
		vp[i].MinDepth = 0.0f;
		vp[i].MaxDepth = 1.0f;
	}
	
    vp[0].TopLeftX = 0;
    vp[0].TopLeftY = 0;

    vp[1].TopLeftX = t_HalfWidth;
    vp[1].TopLeftY = 0;

    vp[2].TopLeftX = 0;
    vp[2].TopLeftY = t_HalfHeight;
	
    vp[3].TopLeftX = t_HalfWidth;
    vp[3].TopLeftY = t_HalfHeight;*/

	D3D11_VIEWPORT vp;

	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_DeviceContext->RSSetViewports( 1, &vp );
}

HRESULT GraphicEngine::CreateRasterizers()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC desc;
	
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;  //TODO
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;

	hr = m_Device->CreateRasterizerState(&desc,&m_RasterizerStateNormal);
	if( FAILED( hr ) )
		return hr;

	desc.FillMode = D3D11_FILL_WIREFRAME;

	hr = m_Device->CreateRasterizerState(&desc,&m_RasterizerStateWireframe);
	if( FAILED( hr ) )
		return hr;


	m_DeviceContext->RSSetState(m_RasterizerStateNormal);

	return hr;
}

HRESULT GraphicEngine::InitializeDepthAndDepthStates()
{
	HRESULT hr = S_OK;

	// Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = m_Width;
    descDepth.Height = m_Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
	
	hr = m_Device->CreateTexture2D( &descDepth, nullptr, &m_DepthStencil );
	if( FAILED( hr ) )
        return hr;

	 D3D11_DEPTH_STENCIL_VIEW_DESC t_DescDSV;
	ZeroMemory( &t_DescDSV, sizeof(t_DescDSV) );
    t_DescDSV.Format = descDepth.Format;
    t_DescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    t_DescDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView( m_DepthStencil, &t_DescDSV, &m_DepthStencilView );
	//not sure if I can release the depth if I wanted?


	//create states
	D3D11_DEPTH_STENCIL_DESC t_DsDesc;

	// Depth test parameters
	t_DsDesc.DepthEnable = true;
	t_DsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	t_DsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	t_DsDesc.StencilEnable = true;
	t_DsDesc.StencilReadMask = 0xFF;
	t_DsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	t_DsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	t_DsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	t_DsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	t_DsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	t_DsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	t_DsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	t_DsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	t_DsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state

	hr = m_Device->CreateDepthStencilState(&t_DsDesc, &m_DepthStateOn);
	if( FAILED( hr ) )
        return hr;

	t_DsDesc.DepthEnable = false;
	t_DsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO ;

	hr = m_Device->CreateDepthStencilState(&t_DsDesc, &m_DepthStateOff);
	if( FAILED( hr ) )
        return hr;

	t_DsDesc.DepthEnable = true;
	t_DsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	t_DsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_Device->CreateDepthStencilState(&t_DsDesc, &m_DepthStateNoWrite);

	return hr;
}

HRESULT GraphicEngine::InitializeBlendAndBlendStates()
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC t_Blend_desc;
	t_Blend_desc.AlphaToCoverageEnable = false;
	t_Blend_desc.IndependentBlendEnable = false;
	

	t_Blend_desc.RenderTarget[0].BlendEnable = true;
	t_Blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	t_Blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	t_Blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	t_Blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	t_Blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	t_Blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	t_Blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; 


	hr = m_Device->CreateBlendState( &t_Blend_desc, &m_BlendStateOn);
	if( FAILED( hr ) )
		return hr;

	t_Blend_desc.RenderTarget[0].BlendEnable = false;
	hr = m_Device->CreateBlendState( &t_Blend_desc, &m_BlendStateOff);
	if( FAILED( hr ) )
		return hr;

	float t_BlendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_DeviceContext->OMSetBlendState(m_BlendStateOff, t_BlendFactors, 0xffffffff);

	return hr;
}

HRESULT GraphicEngine::InitializeShaders()
{
	HRESULT hr = S_OK;

	/*ID3D11ComputeShader* t_TileDeferredCS;
	hr = m_ShaderLoader->CreateComputeShader(L"DeferredRenderingCS.hlsl", "TileDeferredCS", "cs_5_0" , m_Device, &tileDeferredCS);
	if( FAILED( hr ) )
		return hr;

	computeShaders.push_back(tileDeferredCS);

	deviceContext->CSSetShader(computeShaders[0],nullptr,0);

	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = ARRAYSIZE(layout);

	hr = shaderLoader->CreateVertexShaderWithInputLayout(L"NormalVertexShader.hlsl","VS","vs_5_0",device,&vertexShader,layout,numElements,&inputLayout);
	if( FAILED( hr ) )
		return hr;

	vertexShaders.push_back(vertexShader);
	inputLayouts.push_back(inputLayout);

	ID3D11PixelShader* pixelShader;
	hr = shaderLoader->CreatePixelShader(L"NormalPixelShader.hlsl","PS","ps_5_0",device,&pixelShader);
	if( FAILED( hr ))
		return hr;
	pixelShaders.push_back(pixelShader);

	ShaderProgram newProgram;
	newProgram.vertexShader = vertexShaders.size() - 1;
	newProgram.domainShader = -1;
	newProgram.hullShader = -1;
	newProgram.geometryShader = -1;
	newProgram.pixelShader = pixelShaders.size() - 1;

	shaderPrograms.push_back(newProgram);*/

	//fix  shaders here

	return hr;
}

HRESULT GraphicEngine::InitializeConstantBuffers()
{
	HRESULT hr = S_OK;
	
	//D3D11_BUFFER_DESC bd;
	//ZeroMemory( &bd, sizeof(bd) );
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(ConstantBuffer);
 //   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 //   bd.CPUAccessFlags = 0;
	//hr = device->CreateBuffer( &bd, nullptr, &constantBuffer );
 //   if( FAILED( hr ) )
 //       return hr;

	//bd.ByteWidth = sizeof(PerFrameBuffer);
	//hr = device->CreateBuffer( &bd, nullptr, &perFrameBuffer );
 //   if( FAILED( hr ) )
 //       return hr;

	//bd.ByteWidth = sizeof(PerObjectBuffer);
	//hr = device->CreateBuffer( &bd, nullptr, &perObjectBuffer );
 //   if( FAILED( hr ) )
 //       return hr;

	//
	//lights.resize(MAX_NUM_OF_LIGHTS);
	///*bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//bd.Usage = D3D11_USAGE_DYNAMIC;*/
	//bd.ByteWidth = sizeof(Light) * lights.size();
	//bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//bd.StructureByteStride = sizeof(Light);
	//bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//D3D11_SUBRESOURCE_DATA vinitDataA;
	//vinitDataA.pSysMem = &lights[0];

	//hr = device->CreateBuffer(&bd, &vinitDataA, &lightBuffer);
	//if( FAILED(hr))
	//	return hr;

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	//srvDesc.BufferEx.FirstElement = 0;
	//srvDesc.BufferEx.Flags = 0;
	//srvDesc.BufferEx.NumElements = lights.size()/*MAX_NUM_OF_LIGHTS*/;
	//
	//hr = device->CreateShaderResourceView(lightBuffer,&srvDesc,&lightBufferSRV);

	//ConstantBuffer cb;
	//cb.camNearFar = XMFLOAT2(0,0);
	//cb.screenDimensions = XMFLOAT2(width,height);
	//cb.lightCount = lights.size();
	//cb.useFog = 0;
	//cb.filler = 0;
	//cb.filler2 = 0;
	//
	//deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);

	//deviceContext->VSSetConstantBuffers(0,1,&constantBuffer);
	//deviceContext->VSSetConstantBuffers(1,1,&perFrameBuffer);
	//deviceContext->VSSetConstantBuffers(2,1,&perObjectBuffer);

	//deviceContext->HSSetConstantBuffers(1,1,&perFrameBuffer);

	//deviceContext->DSSetConstantBuffers(1,1,&perFrameBuffer);

	//deviceContext->GSSetConstantBuffers(1,1, &perFrameBuffer);
	//deviceContext->GSSetConstantBuffers(2,1,&perObjectBuffer);

	//deviceContext->PSSetConstantBuffers(2,1,&perObjectBuffer);

	//deviceContext->CSSetConstantBuffers(0,1,&constantBuffer);
	//deviceContext->CSSetConstantBuffers(1,1,&perFrameBuffer);



	return hr;
}

HRESULT GraphicEngine::InitializeGBuffers()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //changed from format DXGI_FORMAT_R16G16B16A16_UNORM
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	

	D3D11_SHADER_RESOURCE_VIEW_DESC t_SrvDesc;
	t_SrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	t_SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	t_SrvDesc.Texture2D.MostDetailedMip = 0;
	t_SrvDesc.Texture2D.MipLevels = 1;


	for (int i = 0; i < 3; i++)
	{
		ID3D11Texture2D* t_Texture = 0;

		hr = m_Device->CreateTexture2D(&desc, 0, &t_Texture);
		if( FAILED( hr ) )
			return hr;

		hr = m_Device->CreateShaderResourceView(t_Texture, &t_SrvDesc, &m_GbufferShaderResource[i]);
		if( FAILED( hr) )
			return hr;

		hr = m_Device->CreateRenderTargetView( t_Texture, nullptr, &m_GbufferTargetViews[i] );
		if( FAILED( hr ) )
			return hr;

		t_Texture->Release();

	}

	
	// Views save a reference to the texture so we can release our reference.
	
	return hr;
}

HRESULT GraphicEngine::InitializeSamplerState()
{
	HRESULT hr = S_OK;
	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_Device->CreateSamplerState( &sampDesc, &m_SamplerStateWrap);
	if( FAILED(hr) )
		return hr;

	m_DeviceContext->VSSetSamplers(0,1,&m_SamplerStateWrap);
	m_DeviceContext->HSSetSamplers(0,1,&m_SamplerStateWrap);
	m_DeviceContext->DSSetSamplers(0,1,&m_SamplerStateWrap);
	m_DeviceContext->GSSetSamplers(0,1,&m_SamplerStateWrap);
	m_DeviceContext->PSSetSamplers(0,1,&m_SamplerStateWrap);
	
	return hr;
}

//==========Entity functions=================//

HRESULT GraphicEngine::LoadMesh(std::vector<UINT> &o_DrawPieceIDs)
{
	//fix
	HRESULT hr = S_OK;

	std::vector<std::vector<SimpleVertex>> t_VertexGroupLists;
	/*hr = meshLoader->ReadObjFile(objFileName,device,vertexGroupLists,1.0f);
	if( FAILED( hr ))
		return hr;*/
	
	D3D11_BUFFER_DESC vbd;
	ZeroMemory( &vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	

	D3D11_SUBRESOURCE_DATA t_VinitData;
	ZeroMemory( &t_VinitData, sizeof(t_VinitData));

	for (int i = 0; i < t_VertexGroupLists.size(); i++)
	{
		//create the buffer
		ID3D11Buffer* t_NewVertexBuffer;
		std::vector<SimpleVertex> t_vertexList = t_VertexGroupLists[i];
		vbd.ByteWidth = sizeof(SimpleVertex) * t_vertexList.size();
		t_VinitData.pSysMem = &t_vertexList[0];
		hr = (m_Device->CreateBuffer(&vbd, &t_VinitData, &t_NewVertexBuffer));
		if( FAILED( hr ))
			return hr;

		//add to the graphics buffer list
		VertexBufferWithNOV t_NewVert;
		t_NewVert.vertexBuffer = t_NewVertexBuffer;
		t_NewVert.numberOfVertices = t_vertexList.size();
		m_VertexBuffers.push_back(t_NewVert);

		//create a new draw piece and add to output
		DrawPiece t_DrawPiece;
		t_DrawPiece.vertexBufferID = (m_VertexBuffers.size() - 1);

		m_DrawPieces.push_back(t_DrawPiece);

		o_DrawPieceIDs.push_back(m_DrawPieces.size() -1);
	}

	return hr;
}

HRESULT GraphicEngine::AddTextureToDrawPiece(UINT p_DrawPieceID,UINT p_TextureID,TextureType p_TextureType)
{
	if (p_DrawPieceID >= m_DrawPieces.size() || p_TextureID >= m_Textures.size())
	{
		return E_FAIL;
	}

	switch (p_TextureType)
	{
	case GraphicEngine::DIFFUSE:
		m_DrawPieces[p_DrawPieceID].diffuseTID = p_TextureID;
		break;
	case GraphicEngine::NORMAL:
		m_DrawPieces[p_DrawPieceID].normalTID = p_TextureID;
		break;
	case GraphicEngine::GLOW:
		m_DrawPieces[p_DrawPieceID].glowTID = p_TextureID;
		break;
	case GraphicEngine::SPECULAR:
		m_DrawPieces[p_DrawPieceID].specularTID = p_TextureID;
		break;
	default:
		break;
	}
	
	return S_OK;
}

HRESULT GraphicEngine::CreateObject(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID)
{
	try
	{
		DrawObject* t_NewDrawObject = new DrawObject();
		t_NewDrawObject->piecesID = p_DrawPieceIDs;
	

		//hash object
		std::hash<DrawObject*> T_Hashi;
		o_ObjectID = T_Hashi(t_NewDrawObject);

	
		if (m_DrawOjbects[o_ObjectID] == nullptr)
		{
			m_DrawOjbects[o_ObjectID] = t_NewDrawObject;
		}
	}
	catch( std::exception e )
	{
		MessageBox( nullptr, L"Catched exeption when attempting to hash and allocate new object", L"ErrorMessage", MB_OK );
		return E_FAIL;
	}
	

	if (addToDrawNow)
	{
		//add code
	}
}

HRESULT GraphicEngine::AddObjectLight(UINT p_ObjectID ,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius, UINT &o_LightID)
{
	if (m_DrawOjbects[p_ObjectID] != nullptr)
	{
		m_DrawOjbects[p_ObjectID]->lights.push_back(Light(p_Position,p_Radius,p_Color,0));
		o_LightID = m_DrawOjbects[p_ObjectID]->lights.size() -1;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT GraphicEngine::ChangeObjectsLight(UINT p_ObjectID, UINT p_LightID,XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius)
{
	if (m_DrawOjbects[p_ObjectID] != nullptr)
	{
		if (p_LightID < m_DrawOjbects[p_ObjectID]->lights.size())
		{
			m_DrawOjbects[p_ObjectID]->lights[p_LightID] = Light(p_Position,p_Radius,p_Color,0);
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT GraphicEngine::MoveObject(UINT p_ObjectID, CXMMATRIX p_Matrix)
{
	if (m_DrawOjbects[p_ObjectID] != nullptr)
	{
		 XMStoreFloat4x4(&m_DrawOjbects[p_ObjectID]->worldMatrix, p_Matrix);

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}


//==========Texture functions=================//
HRESULT GraphicEngine::LoadTexture(const wchar_t * p_FileName, UINT &o_TextureID)
{
	HRESULT hr = S_OK;
	ID3D11ShaderResourceView* t_NewSRV;
	//hr = CreateDDSTextureFromFile(m_Device, p_FileName, nullptr, &t_NewSRV);
	if( FAILED( hr ))
		return hr;

	m_Textures.push_back(t_NewSRV);

	o_TextureID = m_Textures.size() - 1;

	return hr;
}

//==========Light functions=================//

void GraphicEngine::CreateStaticLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius)
{
	Light t_NewLight(p_Position,p_Radius,p_Color,0);
	m_StaticLights.push_back(t_NewLight);
}

HRESULT GraphicEngine::CreateDynamicLight(XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius, UINT &o_LightID)
{
	try
	{
		Light* t_NewLight = new Light(p_Position,p_Radius,p_Color,0);

		std::hash<Light*> t_Hashii;

		o_LightID = t_Hashii(t_NewLight);

		if (m_DynamicLights[o_LightID] == nullptr)
		{
			m_DynamicLights[o_LightID] = t_NewLight;
		}
		
	}
	catch( std::exception e )
	{
		MessageBox( nullptr, L"Catched exeption when attempting to hash and allocate new object", L"ErrorMessage", MB_OK );
		return E_FAIL;
	}
	return S_OK;
}

HRESULT GraphicEngine::UpdateDynamicLight(UINT p_LightID, XMFLOAT3 p_Position, XMFLOAT3 p_Color, float p_Radius)
{
	if (m_DynamicLights[p_LightID] != nullptr)
	{
		m_DynamicLights[p_LightID]->color = p_Color;
		m_DynamicLights[p_LightID]->position = p_Position;
		m_DynamicLights[p_LightID]->radius = p_Radius;

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//==========HUD functions=================//
void GraphicEngine::AddHudObject()
{
	
}
void GraphicEngine::CreatehudObject()
{

}
void GraphicEngine::UseHud()
{

}
void GraphicEngine::LoadHud()
{

}

//==========Camera functions=================//

void GraphicEngine::CreateCamera()
{

}

void GraphicEngine::MoveCamera()
{

}

void GraphicEngine::UseCamera()
{

}
