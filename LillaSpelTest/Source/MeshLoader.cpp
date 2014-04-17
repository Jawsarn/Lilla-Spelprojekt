#include "MeshLoader.h"


MeshLoader::MeshLoader(void)
{
}


MeshLoader::~MeshLoader(void)
{
}
HRESULT MeshLoader::ReadObjFile(std::string filename,		//.obj filename
								ID3D11Device* device,std::vector<std::vector<std::vector<SimpleVertex>>> &vertices, float scale)
{
	HRESULT hr = S_OK;



	std::vector<XMFLOAT3> temp_vertices;
	std::vector<XMFLOAT2> temp_uvs;
	std::vector<XMFLOAT3> temp_normals;
	int t_nrEntity=0;
	int t_nrObjKind=0;
	int t_objIndex=0;
	//SimpleVertex t_vertices;


	FILE * file;
	fopen_s(&file,filename.c_str(), "r");



	if( file == NULL )
	{
		printf("Impossible to open the file !\n");
		return E_FAIL;
	}

	while(true)
	{
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}

		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			XMFLOAT3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			vertex.z = vertex.z * -1.0f;
			temp_vertices.push_back(XMFLOAT3(vertex.x*scale,vertex.y*scale,vertex.z*scale));
		}
		else if(strcmp( lineHeader, "vt" ) == 0 )
		{
			XMFLOAT2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = 1.0f - uv.y;
			temp_uvs.push_back(uv);
			// else : parse lineHeader
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			XMFLOAT3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			normal.z = normal.z * -1.0f;
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			//std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
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
				t_vertices.Pos.x = temp_vertices[vertexIndex[i-1]].x;
				t_vertices.Pos.y = temp_vertices[vertexIndex[i-1]].y;
				t_vertices.Pos.z = temp_vertices[vertexIndex[i-1]].z;
				t_vertices.Norm.x = temp_normals[normalIndex[i-1]].x;
				t_vertices.Norm.y = temp_normals[normalIndex[i-1]].y;
				t_vertices.Norm.z = temp_normals[normalIndex[i-1]].z;
				t_vertices.Tex.x = temp_uvs[uvIndex[i-1]].x;
				t_vertices.Tex.y = temp_uvs[uvIndex[i-1]].y;
				vertices[t_nrEntity][t_nrObjKind].push_back(t_vertices);
			}			
		}

		else if( strcmp( lineHeader, "usemtl" ) == 0 )
		{
			char temp[128];
			int outp = fscanf_s(file, "%s\n", temp, _countof(temp));

			if(outp != 1)
			{
				return E_FAIL;
			}
			t_nrObjKind++;
		}

		else if( strcmp( lineHeader, "g" ) == 0 )
		{
			char temp[128];
			int outp = fscanf_s(file, "%s\n", temp, _countof(temp));

			if(outp != 1)
			{
				return E_FAIL;
			}
			t_nrEntity++;
			t_nrObjKind=0;
		}

		//	else if( strcmp( lineHeader, "g" ) == 0 )
		//	{
		//		char temp[128];
		//		int outp = fscanf_s(file, "%s\n", temp, _countof(temp));

		//		if(outp != 1)
		//		{
		//			return E_FAIL;
		//		}
		//		if(tempGroup.groupName == "") //if first group
		//		{
		//			tempGroup.groupName = temp;
		//		}
		//		else
		//		{
		//			groups.push_back(tempGroup);
		//			tempGroup.normalIndices.clear();
		//			tempGroup.uvIndices.clear();
		//			tempGroup.vertexIndices.clear();
		//			tempGroup.materialName = "";
		//			tempGroup.groupName = temp;
		//		}
		//	}
		//	else if( strcmp( lineHeader, "s" ) == 0 )
		//	{
		//		//save side (could be averaged)
		//	}
		//}
		//groups.push_back(tempGroup);

		//for (int i = 0; i < groups.size(); i++)
		//{
		//	if(groups[i].vertexIndices.size() > 0)
		//	{
		//		std::vector<SimpleVertex> tempVerticeList;
		//		std::vector <XMFLOAT3>  out_vertices;
		//		std::vector <XMFLOAT2>  out_uvs;
		//		std::vector <XMFLOAT3>  out_normals;


		//		for(unsigned int j = 0; j< groups[i].vertexIndices.size(); j++)
		//		{
		//			unsigned int vertexIndex = groups[i].vertexIndices[j];
		//			XMFLOAT3  vertex = temp_vertices[vertexIndex - 1];
		//			out_vertices.push_back(vertex);
		//		}
		//		for(unsigned int j = 0; j<groups[i].uvIndices.size(); j++)
		//		{
		//			unsigned int uvIndex = groups[i].uvIndices[j];
		//			XMFLOAT2  uv = temp_uvs[uvIndex - 1];
		//			out_uvs.push_back(uv);
		//		}
		//		for(unsigned int j = 0; j<groups[i].normalIndices.size(); j++)
		//		{
		//			unsigned int normalIndex = groups[i].normalIndices[j];
		//			XMFLOAT3  normal = temp_normals[normalIndex - 1];
		//			out_normals.push_back(normal);

		//		}
		//		for (int j = 0; j < out_vertices.size(); j++)
		//		{
		//			SimpleVertex temp;
		//			temp.Pos = out_vertices[j];
		//			temp.Tex = out_uvs[j];
		//			temp.Norm = out_normals[j];

		//			tempVerticeList.push_back(temp);
		//		}
		//		vertices.push_back(tempVerticeList);
		//	}
		//}

		fclose(file);

		return hr;
	}
}