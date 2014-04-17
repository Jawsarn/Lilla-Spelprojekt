#pragma once

#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "GraphicStructs.h"
//#include "VertexHelper.h"

using namespace DirectX;

class MeshLoader
{
public:
	MeshLoader(void);
	~MeshLoader(void);


	HRESULT ReadObjFile(std::string p_filename,	std::vector<std::vector<SimpleVertex>> &o_vertices, float p_scale);	
};
