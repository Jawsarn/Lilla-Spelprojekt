#include "MeshLoader.h"


MeshLoader::MeshLoader(void)
{
}


MeshLoader::~MeshLoader(void)
{
}
HRESULT MeshLoader::ReadObjFile(std::string p_filename, std::vector<std::vector<SimpleVertex>> &o_vertices, float p_scale)
{
	HRESULT hr = S_OK;



	std::vector<XMFLOAT3> t_positions;
	std::vector<XMFLOAT2> t_uvs;
	std::vector<XMFLOAT3> t_normals;
	int t_nrEntity=0;


	FILE * t_file;
	fopen_s(&t_file,p_filename.c_str(), "r");



	if( t_file == NULL )
	{
		printf("Impossible to open the t_file !\n");
		return E_FAIL;
	}

	while(true)
	{
		char t_lineHeader[128];

		// read the first word of the line
		int res = fscanf_s(t_file, "%s", t_lineHeader, _countof(t_lineHeader));
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}

		if ( strcmp( t_lineHeader, "v" ) == 0 )
		{
			XMFLOAT3 t_vertex;
			fscanf_s(t_file, "%f %f %f\n", &t_vertex.x, &t_vertex.y, &t_vertex.z );
			t_vertex.z = t_vertex.z * -1.0f;
			t_positions.push_back(XMFLOAT3(t_vertex.x*p_scale,t_vertex.y*p_scale,t_vertex.z*p_scale));
		}
		else if(strcmp( t_lineHeader, "vt" ) == 0 )
		{
			XMFLOAT2 t_uv;
			fscanf_s(t_file, "%f %f\n", &t_uv.x, &t_uv.y );
			t_uv.y = 1.0f - t_uv.y;
			t_uvs.push_back(t_uv);
			// else : parse t_lineHeader
		}
		else if ( strcmp( t_lineHeader, "vn" ) == 0 )
		{
			XMFLOAT3 t_nrml;
			fscanf_s(t_file, "%f %f %f\n", &t_nrml.x, &t_nrml.y, &t_nrml.z );
			t_nrml.z = t_nrml.z * -1.0f;
			t_normals.push_back(t_nrml);
		}
		else if ( strcmp( t_lineHeader, "f" ) == 0 )
		{
			//std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(t_file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return E_FAIL;
			}
			SimpleVertex t_vertices;
			for (int i=0;i<3;i++)
			{
				t_vertices.position.x = t_positions[vertexIndex[i]-1].x;
				t_vertices.position.y = t_positions[vertexIndex[i]-1].y;
				t_vertices.position.z = t_positions[vertexIndex[i]-1].z;
				t_vertices.normal.x = t_normals[normalIndex[i]-1].x;
				t_vertices.normal.y = t_normals[normalIndex[i]-1].y;
				t_vertices.normal.z = t_normals[normalIndex[i]-1].z;
				t_vertices.textureUV.x = t_uvs[uvIndex[i]-1].x;
				t_vertices.textureUV.y = t_uvs[uvIndex[i]-1].y;
				o_vertices[t_nrEntity-1].push_back(t_vertices);
			}			
		}

		else if( strcmp( t_lineHeader, "usemtl" ) == 0 )
		{
			char t_[128];
			int outp = fscanf_s(t_file, "%s\n", t_, _countof(t_));

			if(outp != 1)
			{
				return E_FAIL;
			}
		}

		else if( strcmp( t_lineHeader, "g" ) == 0 )
		{
			char t_[128];
			int outp = fscanf_s(t_file, "%s\n", t_, _countof(t_));

			if(outp != 1)
			{
				return E_FAIL;
			}
			t_nrEntity++;
			o_vertices.resize(t_nrEntity);
		}

	}
			fclose(t_file);

		return hr;
}