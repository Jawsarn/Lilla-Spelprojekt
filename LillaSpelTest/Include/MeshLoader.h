#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
//#include "VertexHelper.h"

using namespace DirectX;

class MeshLoader
{
public:
	MeshLoader(void);
	~MeshLoader(void);

		struct SimpleVertex //structcclass
{
	XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT2 Tex;
};

	HRESULT ReadObjFile(std::string filename,
						ID3D11Device* device,
						std::vector<std::vector<std::vector<SimpleVertex>>> &vertices, 
						float scale);	
};
