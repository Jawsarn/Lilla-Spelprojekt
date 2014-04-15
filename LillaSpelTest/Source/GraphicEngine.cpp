#include "GraphicEngine.h"


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

	//hr = InitializeDepthAndDepthStates();
	//if( FAILED( hr ) )
	//	return hr;

	//hr = InitializeBlendAndBlendStates();
	//if( FAILED( hr ) )
	//	return hr;

	//hr = InitializeShaders();
	//if( FAILED( hr ) )
	//	return hr;

	//hr = InitializeConstantBuffers();
	//if( FAILED( hr ) )
	//	return hr;

	//hr = InitializeGBuffers();
	//if( FAILED( hr ) )
	//	return hr;

	//hr = InitializeSamplerState();
	//if( FAILED( hr) )
	//	return hr;

	/*particleSystem = particleSystem->GetInstance();
	hr = particleSystem->Initialize(device,deviceContext,depthStateOn,depthStateOff, blendStateOn, blendStateOff,perObjectBuffer);
	if( FAILED( hr ) )
		return hr;*/


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
	FLOAT t_HalfWidth = (FLOAT)m_Width/2;
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
    vp[3].TopLeftY = t_HalfHeight;

	m_DeviceContext->RSSetViewports( 4, vp );
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