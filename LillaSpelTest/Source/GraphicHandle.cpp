#include "GraphicHandle.h"
#include "GraphicEngine.h"

GraphicHandle* GraphicHandle::m_Singleton = nullptr;

GraphicHandle* GraphicHandle::GetInstance()
{
	if(m_Singleton == nullptr)
	{
		m_Singleton = new GraphicHandle();
	}
	return m_Singleton;
}

GraphicHandle::GraphicHandle()
{

}
GraphicHandle::~GraphicHandle()
{

}

void GraphicHandle::Initialize(UINT p_Width, UINT p_Height, HWND p_Handle)
{
	m_GraphicEngine = m_GraphicEngine->GetInstance();
	m_GraphicEngine->Initialize(p_Width, p_Height, p_Handle);

	//load a diffuse texture
	UINT diffuseBollTestTexture;
	m_GraphicEngine->LoadTexture(L"TubeTexture1.dds", diffuseBollTestTexture);


	
	//load a ship mesh 
	std::vector<UINT> t_ObjTemp;
	m_GraphicEngine->LoadMesh("curveTest2.obj",t_ObjTemp);
	m_MeshLevels.push_back(t_ObjTemp);

	//add a texture to a ship mesh
	for (int i = 0; i < t_ObjTemp.size(); i++)
	{
		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[i],diffuseBollTestTexture,GraphicEngine::TextureType::DIFFUSE);
	}
	

	//testshppiie
	t_ObjTemp.clear();
	m_GraphicEngine->LoadMesh("spaceship.obj",t_ObjTemp);
	m_MeshShips.push_back(t_ObjTemp);
	m_MeshShips.push_back(t_ObjTemp);
	m_MeshShips.push_back(t_ObjTemp);
	m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	//m_MeshShips.push_back(t_ObjTemp);
	m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],diffuseBollTestTexture,GraphicEngine::TextureType::DIFFUSE);
	
	


	//creating a ship that the player is going to use, move to other place when we've done testing


//////////////////////////////////////////////////////////////TEST!!!!!!!!!!!!!!!!!!!///////////////////////////
	XMMATRIX t_Mat = XMMatrixTranslation(0,0,0);
	XMMATRIX t_World=t_Mat;
	//XMMATRIX t_MajsMat=XMMatrixTranslation(100,100,100);
	XMFLOAT3 t_Color = XMFLOAT3(0,1,0);
	std::vector<int>VilkenVehicle;
	std::vector <XMMATRIX> plajerwurld;
	
	std::vector <XMFLOAT3> plajercullur;
	plajercullur.push_back(t_Color);
	t_Color = XMFLOAT3(1,1,0);
	plajercullur.push_back(t_Color);
	t_Color = XMFLOAT3(1,0,0);
	plajercullur.push_back(t_Color);
	t_Color = XMFLOAT3(0,0,1);
	plajercullur.push_back(t_Color);
	plajerwurld.push_back(t_Mat);
	
	t_Mat = XMMatrixTranslation(10,10,10);
	plajerwurld.push_back(t_Mat);
	t_Mat = XMMatrixTranslation(20,20,20);
	plajerwurld.push_back(t_Mat);
	t_Mat = XMMatrixTranslation(30,30,30);
	plajerwurld.push_back(t_Mat);
	t_Mat = XMMatrixTranslation(100,10,10);

	VilkenVehicle.push_back(0);
	VilkenVehicle.push_back(0);
	VilkenVehicle.push_back(0);
	VilkenVehicle.push_back(0);
	StartGame(0,VilkenVehicle,plajerwurld,plajercullur,t_World,t_Color);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//SelectVehicle();




	//m_GraphicEngine->CreateDrawObject(m_Ships[0],t_Mat, t_Color,true, objectID);


	//UINT objectID1;
	//XMMATRIX t_Mat1 = XMMatrixIdentity();
	//XMFLOAT3 t_Color = XMFLOAT3(0,1,0);
	//m_GraphicEngine->CreateDrawObject(m_Ships[1],t_Mat1, t_Color,true, objectID1);


	//add light to an already existing ship, (note to self maybe, if we have two ships that are the same for two players, same textures will be used, but not same lights, this is ok)
	//second note, add light is in object space
	//UINT playerLightOne; //add to a player light array maybe?
	//m_GraphicEngine->AddObjectLight(objectID, XMFLOAT3(0,-2,0),XMFLOAT3(1,0,0),3, playerLightOne);


	//create a camera, just for testing and stuff, think you'll want to create it later with players tho, because the hud wont rely on camera at first
	
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->CreateCamera(XMFLOAT3(250,0,-100),XMFLOAT3(0,0,1),XMFLOAT3(0,1,0),XM_PIDIV4,p_Width/2,p_Height/2,1.0f,10000, m_CameraID[i]);
	}
	
	//set cameras
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->UseCamera(i,m_CameraID[i]);
	}

	 //not fixed yet, just for testing
	m_GraphicEngine->SetViewportAmount(4);


	//m_GraphicEngine->MoveCamera(m_CameraID[0], 20,0,0,0,0);
	//m_GraphicEngine->MoveCamera(m_CameraID[1], 50,0,0,0,0);
	//m_GraphicEngine->MoveCamera(m_CameraID[2], 100,0,0,0,0);
	//m_GraphicEngine->SetCamera(m_CameraID[3], XMFLOAT3(0, 0, -120),XMFLOAT3(0, 0, 1),XMFLOAT3(0, 1, 0));

	UINT t_LightID;
	m_GraphicEngine->CreateStaticLight(XMFLOAT3(30,0,0), XMFLOAT3(1,0,0), 200, t_LightID);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			
			for (int k = 0; k < 6; k++)
			{
				int r, g, b;
				r = rand() % 2;
				g = rand() % 2;
				b = rand() % 2;

				m_GraphicEngine->CreateStaticLight(XMFLOAT3(-50 + 30*i,30*j,-50 + 30*k), XMFLOAT3(r,g,b), 40, t_LightID);
			}
		}
	}
}

void GraphicHandle::UpdatePlayer(int p_playerID,CXMMATRIX p_matrix)
{
	m_GraphicEngine->MoveObject(p_playerID, p_matrix);
	//uppdatera spelarens mätare cooldownbars(HUD)
}
void GraphicHandle::UpdateSelectVehicle(float p_DeltaTime)
{
	XMMATRIX t_Rotii = XMMatrixRotationY(p_DeltaTime);

	for (int i = 0; i < m_SelectionShipMatrix.size(); i++)
	{
		XMMATRIX t_Tempii = XMMatrixMultiply(t_Rotii,XMLoadFloat4x4(&m_SelectionShipMatrix[i]));
		m_GraphicEngine->MoveObject(m_SelectionShips[i],t_Tempii);
		XMFLOAT4X4 t_Storii;
		XMStoreFloat4x4(&t_Storii,t_Tempii);
		m_SelectionShipMatrix[i] = t_Storii;
	}

}

void GraphicHandle::UpdateCamera(UINT p_CameraLogicID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY)
{
	if (p_CameraLogicID < 5)
	{
		m_GraphicEngine->MoveCamera(m_CameraID[p_CameraLogicID],p_Walk,p_Strafe,p_Hover,p_Pitch,p_RotateY);
	}
}
void GraphicHandle::UpdateCameraVehicleSelection(UINT p_CameraLogicID, UINT p_LookingAtWhatVehicle,float p_DeltaTime)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,8*m_MeshShips.size());
		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*p_LookingAtWhatVehicle);
		XMMATRIX t_Rot2 = XMMatrixRotationX(-XM_PIDIV4/8);
		
		t_Rot = XMMatrixMultiply(t_Rot,t_Rot2);//sätter ihop rotationerna
		t_Rot = XMMatrixMultiply(t_Rot, t_Tempura);//roterar matrisen
		
		t_Tempura = XMMatrixRotationY(XM_PI);//vänder med 180 grader

		t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);//lägger in den sista rotationen

		
		//t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);

		m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],t_Rot);
		/////////////////////////////////////////////////////////////////////////////////


		

		// t_Tempura = XMMatrixTranslation(0,1,8*m_MeshShips.size());
		//XMMATRIX t_Rot3 = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*(p_LookingAtWhatVehicle+1));
		//XMMATRIX t_Rot2 = XMMatrixRotationX(-XM_PIDIV4/8);
		//
		//t_Rot3 = XMMatrixMultiply(t_Rot3,t_Rot2);//sätter ihop rotationerna
		//t_Rot3 = XMMatrixMultiply(t_Rot3, t_Tempura);//roterar matrisen
		//
		//t_Tempura = XMMatrixRotationY(XM_PI);//vänder med 180 grader

		//t_Rot3 = XMMatrixMultiply(t_Tempura,t_Rot3);//lägger in den sista rotationen


		//t_Rot;//startposition
		//t_Rot3;//slutpositionen




	}
}
//void GraphicHandle::TurnCameraSelection(float p_DeltaTime,CXMMATRIX p_Start, CXMMATRIX p_End,)
//{
//
//}

void GraphicHandle::JohnSetCamera(CXMMATRIX p_World, UINT p_CameraLogicID)
{
	m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],p_World);
}
void GraphicHandle::SetCamera(UINT p_CameraLogicID, XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up)
{
	if (p_CameraLogicID < 5)
	{
		m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID], p_Pos, p_At, p_Up);
	}
}

void GraphicHandle::CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID)
{

	//object id o camera id kan vara samma
	//m_GraphicEngine->CreateDrawObject(p_DrawPieceIDs, p_World,  addToDrawNow,  o_ObjectID);
	m_GraphicEngine->CreateCamera(p_Pos, p_At, p_Up, p_FieldOfView, p_Width, p_Height, p_NearZ, p_FarZ, o_CameraID);
	m_GraphicEngine->CreatehudObject();//itne klar i engine
	//m_GraphicEngine->CreateParticleSystem();//ej klar
}

void GraphicHandle::DrawGame() //test 
{
	m_GraphicEngine->DrawGame();
}
//void GraphicHandle::CreateShip(int p_PlayerIndex, XMFLOAT3 p_Color, CXMMATRIX p_PlayerWorld,int p_ShipIndex)
//{
	//m_GraphicEngine->CreateDrawObject(m_Ships[p_ShipIndex],p_PlayerWorld, p_Color,true, m_Player[p_PlayerIndex]);
//}
void GraphicHandle::CreateLight(int p_PlayerIndex,XMFLOAT3 p_Color,UINT p_ObjectId, LightStruct &p_LightStruct)
{
	m_GraphicEngine->AddObjectLight(p_ObjectId,p_LightStruct.m_Position,p_LightStruct.m_Color,p_LightStruct.m_Radius,p_LightStruct.m_LightID);
}

void GraphicHandle::StartGame(int p_WhatLevel,
							  std::vector<int> p_WhatVehicle,
							  std::vector
							  <XMMATRIX> p_PlayerWorld,
							  std::vector<XMFLOAT3>p_Color,
							  CXMMATRIX p_LevelWorld,
							  XMFLOAT3 p_LevelColor)
{
	for (int i = 0; i < p_WhatVehicle.size(); i++)
	{
		m_GraphicEngine->CreateDrawObject(m_MeshShips[p_WhatVehicle[i]],
			p_PlayerWorld[i],
			p_Color[i],true, 
			m_Player[i]);
	}
	m_GraphicEngine->CreateDrawObject(m_MeshLevels[p_WhatLevel],p_LevelWorld,p_Color[0],true,m_CurrentLevel);
	//StartGame(0,VilkenVehicle,plajerwurld,plajercullur,t_Mat,t_Color);
}
void GraphicHandle::SelectVehicle()
{

	XMMATRIX t_WorldMat = XMMatrixTranslation(0,0,5*m_MeshShips.size());

	XMFLOAT3 t_Color = XMFLOAT3(0,1,0);

	for (int i = 0; i < m_MeshShips.size(); i++)
	{
		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*i);

		t_Rot = XMMatrixMultiply(t_WorldMat, t_Rot);

		m_GraphicEngine->CreateDrawObject(m_MeshShips[i],
			t_Rot,
			t_Color,
			true, 
			m_SelectionShips[i]);

		XMFLOAT4X4 t_Tempus;
		XMStoreFloat4x4(&t_Tempus, t_Rot);
		m_SelectionShipMatrix.push_back(t_Tempus);
	}
}
int GraphicHandle::GetAmountOfVehicles()
{
	return m_MeshShips.size();
}
//void GraphicHandle::SelectVehicle()
//{
//	XMMATRIX m_SelectionShipMatrix = XMMatrixTranslation(0,0,7*m_MeshShips.size());
//	XMFLOAT3 t_Color = XMFLOAT3(0,1,0);
//
//	for (int i = 0; i < m_MeshShips.size(); i++)
//	{
//		
//		t_Mat*= XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*i);
//
//		m_GraphicEngine->CreateDrawObject(m_MeshShips[i],
//			t_Mat,
//			t_Color,
//			true, 
//			m_SelectionShips[i]);
//
//		
//	}
//}
