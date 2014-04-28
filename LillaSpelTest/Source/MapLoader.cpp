#include "MapLoader.h"

MapLoader::MapLoader(void)
{
	m_mathHelper = MathHelper();
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


vector<MapNode*>* MapLoader::LoadMap(string p_mapName)
{

	//silly stuff. Apparently one does not simply add strings together
	string t_centerSplineString = AddStrings(p_mapName, "CenterSpline.obj");
	string t_edgeSplineString = AddStrings(p_mapName, "EdgeSpline.obj");
	string t_holeBoxString = AddStrings(p_mapName, "HoleBoxes.obj");
	string t_wallBoxString = AddStrings(p_mapName, "WallBoxes.obj");		///////////TO BE CHANGED. RATHER SILLY AS IT IS//////////////

	//Load all information necessary for node creation
	vector<vector<XMFLOAT3>> t_centerPositions = LoadLogicalObj(t_centerSplineString);
	vector<vector<XMFLOAT3>> t_edgePositions = LoadLogicalObj(t_edgeSplineString);

	//Load all information necessary for static object creation
	vector<vector<XMFLOAT3>> t_holeBoxPositions = LoadLogicalObj(t_holeBoxString);
	vector<vector<XMFLOAT3>> t_wallBoxPositions = LoadLogicalObj(t_wallBoxString);



	//Create Nodes and stores them in member variable
	LoadNodes(&t_centerPositions[0], &t_edgePositions[0]);

	//Creates Hole boxes and stores them in StaticObj list member variable
	LoadBoxes(&t_holeBoxPositions, HOLE);

	//Creates Wall boxes and stores them in StaticObj list member variable
	LoadBoxes(&t_wallBoxPositions, WALL);

	AssignBoxesToNodes();

	return &m_logicalMap;
}

void MapLoader::AssignBoxesToNodes()
{
	//iterate all boxes over each mapnode
	for (int i = 0; i < m_logicalMap.size(); i++)
	{
		//////////////////generate local box for MapNode i

		XMVECTOR t_position = XMLoadFloat3(&m_logicalMap[i]->m_position);
		XMVECTOR t_target = XMLoadFloat3(&m_logicalMap[i]->m_normal);
		//create up vector. Here called radius, as up orientation doesn't really matter in a tube. 
		XMFLOAT3 t_float3 = XMFLOAT3(0,0,1);
		XMVECTOR t_vector= XMLoadFloat3(&t_float3);
		XMVECTOR t_radiusVector = XMVector3Cross(t_vector, t_target);

		///////creates orientation quaternion
		XMFLOAT4 t_boxOrientationQuaternion = XMFLOAT4(0,0,0,1);
		//creates transformation matrix for orientation quaternion. Using lookat
		XMMATRIX t_boxOrientationMatrix = XMMatrixLookAtLH(t_position, t_target, t_radiusVector);
		XMVECTOR t_boxOrientationVector = XMLoadFloat4(&t_boxOrientationQuaternion);
		t_boxOrientationVector = XMVector4Transform(t_boxOrientationVector, t_boxOrientationMatrix);
		t_boxOrientationVector = XMVector4Normalize(t_boxOrientationVector);
		XMStoreFloat4(&t_boxOrientationQuaternion, t_boxOrientationVector);

		///////creates local collision box
		//creates box center
		XMFLOAT3 t_boxCenter = m_mathHelper.VecAddVec(m_logicalMap[i]->m_position, m_mathHelper.FloatMultiVec(0.5f, m_logicalMap[i]->m_normal));
		//checks which radius is largest. Don't wanna miss boxes in case of big slopes
		float radius = m_logicalMap[i]->m_radius;
		if(i<m_logicalMap.size()-1 && radius < m_logicalMap[i]->m_nextNode->m_radius)
			radius = m_logicalMap[i]->m_nextNode->m_radius;
		//uses radius and abs(normal) to create extents. (Normal is not normalized; it is the vector to the next node)
		XMFLOAT3 t_boxExtents = XMFLOAT3(radius, radius, m_mathHelper.Abs(m_logicalMap[i]->m_normal));
		BoundingOrientedBox t_box = BoundingOrientedBox(t_boxCenter, t_boxExtents, t_boxOrientationQuaternion);
		for (int j = 0; j < m_boxes.size(); j++)
		{
			//BoundingOrientedBox derp = m_boxes[j];
			if(t_box.Intersects(*m_boxes[j]->GetBox()))
			{
				m_logicalMap[i]->m_staticObjs.push_back(m_boxes[j]);
			}
			else
			{
				int k = 0;
				k++;
			}
		}
	}
}

void MapLoader::LoadBoxes(vector<vector<XMFLOAT3>>* p_boxCornerPositions, ObjectType p_objectType)
{
	//iterate over each set of box corners
	for (int i = 0; i < p_boxCornerPositions->size(); i++)
	{
		BoundingOrientedBox t_box = BoundingOrientedBox();
		t_box.CreateFromPoints(t_box, 8, &p_boxCornerPositions->at(i).at(0), sizeof(XMFLOAT3));
		m_boxes.push_back(new StaticObj(p_objectType, t_box));
	}

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
			m_logicalMap[i]->m_radiusVector = t_radiusVector;
			XMStoreFloat(&m_logicalMap[i]->m_radius, XMVector3Length(XMLoadFloat3(&t_radiusVector)));			//somehow gets the absolute value of the radius vector
	}
	m_logicalMap[m_logicalMap.size()-1]->m_nextNode = m_logicalMap[0];
	XMFLOAT3 v = m_logicalMap[m_logicalMap.size()-1]->m_position;
	XMFLOAT3 s = m_logicalMap[0]->m_position;
	m_logicalMap[m_logicalMap.size()-1]->m_normal = XMFLOAT3(s.x-v.x, s.y-v.y, s.z-v.z);
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
		if ( strcmp( lineHeader, "g" ) == 0 || strcmp( lineHeader, "o" ) == 0)
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

	fclose(file);
	return t_positions;
}


XMVECTOR GetUpVector(XMVECTOR p_normal, XMVECTOR p_radius)
{
	return XMVector3Cross(p_normal, p_radius);
}
