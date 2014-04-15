#include "MapLoader.h"


MapLoader::MapLoader(void)
{

}


MapLoader::~MapLoader(void)
{

}


vector<MapNode*> MapLoader::LoadMap(string mapName)
{

	vector<MapNode*> r_logicalMap;
	//silly stuff. Apparently one does not simply add strings together
	string t_centerSplineString = mapName;
	t_centerSplineString += (string)("CenterSpline.obj");
	string t_edgeSplineString = mapName;
	t_edgeSplineString += (string)("EdgeSpline.obj");


	vector<vector<XMFLOAT3>> t_centerPositions = LoadLogicalObj(t_centerSplineString);
	vector<vector<XMFLOAT3>> t_edgePositions = LoadLogicalObj(t_edgeSplineString);

	for (int i = 0; i < t_centerPositions[0].size(); i++)
	{
		r_logicalMap.push_back(new MapNode());
		r_logicalMap[i]->m_position = t_centerPositions[0][i];

		if(i>0)
		{
			//fixes normal. Current position minus previous position. Previous' node normal points to current node's position
			//INTENTIONALLY NOT NORMALIZED
			XMFLOAT3 v = r_logicalMap[i-1]->m_position;
			XMFLOAT3 s = r_logicalMap[i]->m_position;
			r_logicalMap[i-1]->m_normal = XMFLOAT3(s.x-v.x, s.y-v.y, s.z-v.z);

			//fixes previous and next node pointers for the nodes
			r_logicalMap[i]->m_previousNode = r_logicalMap[i-1];
			r_logicalMap[i-1]->m_nextNode = r_logicalMap[i];
		}
		
		XMFLOAT3 s = t_centerPositions[0][i];		//somewhat hardcoded zero. The splines are always only one object per .obj
		XMFLOAT3 v = t_edgePositions[0][i];
		XMFLOAT3 t_radiusVector = XMFLOAT3(s.x-v.x, s.y-v.y, s.z-v.z);
		XMStoreFloat(&r_logicalMap[i]->m_radius, XMVector3Length(XMLoadFloat3(&t_radiusVector)));			//somehow gets the absolute value of the radius vector


	}

	return r_logicalMap; 
}

vector<vector<XMFLOAT3>> MapLoader::LoadLogicalObj(string splineName)
{

	FILE * file;
	fopen_s(&file, splineName.c_str(), "r");
	vector<vector<XMFLOAT3>> t_positions;
	int t_objectCounter = 0;

	while(true)
	{
		char lineHeader[128];

		//read the first word of the row
		int res = fscanf(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}
		if ( strcmp( lineHeader, "o" ) == 0 )
		{
			t_objectCounter++;
			t_positions.resize(t_objectCounter);
		}
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			XMFLOAT3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z );
			position.z = position.z * -1.0f;						///////////////directx stuff. Could royally fail us over
			t_positions[t_objectCounter-1].push_back(XMFLOAT3(position.x,position.y,position.z));
		}
	}

	return t_positions;
}



