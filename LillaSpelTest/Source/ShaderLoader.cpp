#include "ShaderLoader.h"
#include <d3dcompiler.h>

ShaderLoader::ShaderLoader(void)
{
}


ShaderLoader::~ShaderLoader(void)
{
}

HRESULT ShaderLoader::CreateVertexShaderWithInputLayout(WCHAR* p_FileName, 
														 LPCSTR p_EntryPoint, 
														 LPCSTR p_ShaderModel, 
														 ID3D11Device* p_Device, 
														 ID3D11VertexShader** o_ShaderOut, 
														 D3D11_INPUT_ELEMENT_DESC* p_Layout,
														 UINT p_NumElements, 
														 ID3D11InputLayout** o_LayoutOut)
{
	HRESULT hr = S_OK;
	
	//compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }


	// Create the vertex shader
	hr = p_Device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, o_ShaderOut );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    
     // Create the input layout
	hr = p_Device->CreateInputLayout( p_Layout, p_NumElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), o_LayoutOut );

	pVSBlob->Release();
	
    return hr;
}


HRESULT ShaderLoader::CreateHullShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11HullShader** o_ShaderOut)
{
	HRESULT hr = S_OK;

	// Compile the hull shader
	ID3DBlob* t_PSBlob = nullptr;
	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &t_PSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the hull shader
	hr = p_Device->CreateHullShader( t_PSBlob->GetBufferPointer(), t_PSBlob->GetBufferSize(), nullptr, o_ShaderOut );
	t_PSBlob->Release();

	return hr;
}
HRESULT ShaderLoader::CreateDomainShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11DomainShader** o_ShaderOut)
{
	HRESULT hr = S_OK;

	// Compile the domain shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the domain shader
	hr = p_Device->CreateDomainShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, o_ShaderOut );
	pPSBlob->Release();

	return hr;
}


#include <fstream>
using namespace std;
HRESULT ShaderLoader::CreateGeometryShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11GeometryShader** o_ShaderOut)
{
	HRESULT hr = S_OK;

	// Compile the geometry shader
	ID3DBlob* p_PSBlob = nullptr;

    ofstream program3data;
    program3data.open("RIGHTHEREYOUGIT.txt");
    program3data.close();
    std::ifstream infile("RIGHTHEREYOUGIT.txt");
    std::ifstream infile2("GraphicNormalGS.hlsl");
    FILE* f;
    fopen_s(&f, "GraphicNormalGS.hlsl", "r");

	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &p_PSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the geometry shader
	hr = p_Device->CreateGeometryShader( p_PSBlob->GetBufferPointer(), p_PSBlob->GetBufferSize(), nullptr, o_ShaderOut );
	p_PSBlob->Release();

    return hr;
}

HRESULT ShaderLoader::CreateGeometryShaderWithOutputDeclaration(WCHAR* p_FileName, 
														 LPCSTR p_EntryPoint, 
														 LPCSTR p_ShaderModel, 
														 ID3D11Device* p_Device, 
														 ID3D11GeometryShader** o_ShaderOut, 
														 D3D11_SO_DECLARATION_ENTRY* decl,
														 UINT numElements)	
{
	//Create the update Geometry shader for output
	HRESULT hr = S_OK;
	ID3DBlob* p_PSBlob = nullptr;

	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &p_PSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }


	hr = p_Device->CreateGeometryShaderWithStreamOutput( p_PSBlob->GetBufferPointer(), p_PSBlob->GetBufferSize(), decl, 
		numElements, NULL, 0, D3D11_SO_NO_RASTERIZED_STREAM, NULL, o_ShaderOut );

	p_PSBlob->Release();
    if( FAILED( hr ) )
        return hr;
}
HRESULT ShaderLoader::CreatePixelShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11PixelShader** o_ShaderOut)
{
	HRESULT hr = S_OK;

	// Compile the pixel shader
	ID3DBlob* p_PSBlob = nullptr;
	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &p_PSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = p_Device->CreatePixelShader( p_PSBlob->GetBufferPointer(), p_PSBlob->GetBufferSize(), nullptr, o_ShaderOut );
	p_PSBlob->Release();

	return hr;
}

HRESULT ShaderLoader::CreateComputeShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11ComputeShader** o_ShaderOut)
{
	HRESULT hr = S_OK;

	// Compile the pixel shader
	ID3DBlob* p_PSBlob = nullptr;
	hr = CompileShaderFromFile( p_FileName, p_EntryPoint, p_ShaderModel, &p_PSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = p_Device->CreateComputeShader( p_PSBlob->GetBufferPointer(), p_PSBlob->GetBufferSize(), nullptr, o_ShaderOut);

	p_PSBlob->Release();

	return hr;
}



HRESULT ShaderLoader::CompileShaderFromFile( WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3DBlob** o_BlobOut )
{
	HRESULT hr = S_OK;

	    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif


	ID3DBlob* t_ErrorBlob = nullptr;
	
    hr = D3DCompileFromFile( p_FileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE , p_EntryPoint, p_ShaderModel, 
        dwShaderFlags, 0, o_BlobOut, &t_ErrorBlob );
	 
    if( FAILED(hr) )
    {
        if( t_ErrorBlob )
        {
            ofstream myfile;
            myfile.open("SHITHAPPENED.txt");
            myfile << (char*)t_ErrorBlob->GetBufferPointer();
            myfile.close();
            OutputDebugStringA((char*)( t_ErrorBlob->GetBufferPointer() ) );
            t_ErrorBlob->Release();
        }
        return hr;
    }
    if( t_ErrorBlob ) t_ErrorBlob->Release();

    return S_OK;
}