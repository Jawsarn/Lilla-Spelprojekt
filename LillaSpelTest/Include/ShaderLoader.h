#pragma once

#include <d3d11_1.h>


class ShaderLoader
{
public:
	ShaderLoader(void);
	~ShaderLoader(void);

	//standard compileShaderFromFile, used for all create shaders
	HRESULT CompileShaderFromFile( WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3DBlob** p_BlobOut );
	
	//creates a shader with a input layout
	HRESULT CreateVertexShaderWithInputLayout(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, 
		ID3D11VertexShader** o_ShaderOut, D3D11_INPUT_ELEMENT_DESC* p_Layout,UINT p_NumElements, ID3D11InputLayout** o_LayoutOut);
	//creates a hull shader
	HRESULT CreateHullShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11HullShader** o_ShaderOut);
	//creates a domain shader
	HRESULT CreateDomainShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11DomainShader** o_ShaderOut);
	//creates a geometry shader
	HRESULT CreateGeometryShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11GeometryShader** o_ShaderOut);
	//creates a pixel shader
	HRESULT CreatePixelShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11PixelShader** o_ShaderOut);
	//creates a compute shader
	HRESULT CreateComputeShader(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, ID3D11ComputeShader** o_ShaderOut);
	//creates a output geometry shader
	HRESULT CreateGeometryShaderWithOutputDeclaration(WCHAR* p_FileName, LPCSTR p_EntryPoint, LPCSTR p_ShaderModel, ID3D11Device* p_Device, 
		ID3D11GeometryShader** o_ShaderOut, D3D11_SO_DECLARATION_ENTRY* p_Decl, UINT p_NumElements);
};

