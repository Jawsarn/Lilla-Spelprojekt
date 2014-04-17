#include "MapLoader.h"


MapLoader::MapLoader(void)
{

}


MapLoader::~MapLoader(void)
{

}

string AddStrings(string p_string1, string p_string2)
{
	string r_string = p_string1;
	r_string += p_string2;
	return r_string;
}


vector<MapNode*> MapLoader::LoadMap(string p_mapName)
{

	vector<MapNode*> r_logicalMap;
	//silly stuff. Apparently one does not simply add strings together
	string t_centerSplineString = AddStrings(p_mapName, "CenterSpline.obj");
	string t_edgeSplineString = AddStrings(p_mapName, "EdgeSpline.obj");
	string t_holeBoxString = AddStrings(p_mapName, "HoleBoxes.obj");
	string t_wallBoxString = AddStrings(p_mapName, "WallBoxes.obj");

	//Load all information necessary for node creation
	vector<vector<XMFLOAT3>> t_centerPositions = LoadLogicalObj(t_centerSplineString);
	vector<vector<XMFLOAT3>> t_edgePositions = LoadLogicalObj(t_edgeSplineString);

	//Load all information necessary for static object creation
	//vector<vector<XMFLOAT3>> t_holeBoxPositions = LoadLogicalObj(t_holeBoxString);
	//vector<vector<XMFLOAT3>> t_wallBoxPositions = LoadLogicalObj(t_wallBoxString);

	//Create Nodes and stores them in member variable
	LoadNodes(&t_centerPositions[0], &t_edgePositions[0]);

	return m_logicalMap;
}

void MapLoader::LoadNodes(vector<XMFLOAT3>* p_centerPositions, vector<XMFLOAT3>* p_edgePositions)
{
		for (int i = 0; i < p_centerPositions->size(); i++)
	{
		m_logicalMap.push_back(new MapNode());
		m_logicalMap[i]->m_position = p_centerPositions->at(i);

		if(i>0)
		{
			//fixes normal. Current position minus previous position. Previous' node normal points to current node's position
			//INTENTIONALLY NOT NORMALIZED
			XMFLOAT3 v = m_logicalMap[i-1]->m_position;
			XMFLOAT3 s = m_logicalMap[i]->m_position;
			m_logicalMap[i-1]->m_normal = XMFLOAT3(s.x-v.x, s.y-v.y, s.z-v.z);

			//fixes previous and next node pointers for the nodes
			m_logicalMap[i]->m_previousNode = m_logicalMap[i-1];
			m_logicalMap[i-1]->m_nextNode = m_logicalMap[i];
		}
		
		XMFLOAT3 s = p_centerPositions->at(i);		
		XMFLOAT3 v = p_edgePositions->at(i);
		XMFLOAT3 t_radiusVector = XMFLOAT3(s.x-v.x, s.y-v.y, s.z-v.z);
		XMStoreFloat(&m_logicalMap[i]->m_radius, XMVector3Length(XMLoadFloat3(&t_radiusVector)));			//somehow gets the absolute value of the radius vector

	}
}



vector<vector<XMFLOAT3>> MapLoader::LoadLogicalObj(string p_objName)
{

	FILE * file;
	fopen_s(&file, p_objName.c_str(), "r");
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


XMVECTOR GetUpVector(XMVECTOR p_normal, XMVECTOR p_radius)
{
	return XMVector3Cross(p_normal, p_radius);
}
