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

		struct SimpleVertex //eric123
{
	XMFLOAT3 m_pos;
	XMFLOAT3 m_norm;
	XMFLOAT2 m_tex;
};

	HRESULT ReadObjFile(std::string p_filename,
						std::vector<std::vector<SimpleVertex>> &o_vertices, 
						float p_scale);	
};
