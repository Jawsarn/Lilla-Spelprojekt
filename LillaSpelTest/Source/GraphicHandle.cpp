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

void GraphicHandle::Initialize(UINT p_Width, UINT p_Height, HWND p_Handle, std::vector <std::string> p_LevelNames)
{


	m_GraphicEngine = m_GraphicEngine->GetInstance();
	m_GraphicEngine->Initialize(p_Width, p_Height, p_Handle);
	SetAmountOfPlayers(4);


	m_WhatLevelBefore=0;


	UINT t_TempurTextur;
	UINT t_TempurTexturNG;





	////initWallTextures

	m_WallNames.push_back("Walls/FirstWall");
	for (int i = 0; i < m_WallNames.size(); i++)
	{
		m_MeshPlayerWall.push_back(InitializeObj(m_WallNames[i]));
	}

	//init skepp
	std::vector<std::string> t_ShipNames;
	t_ShipNames.push_back("Ships/MilleniumKalk");
	t_ShipNames.push_back("Ships/PajFighter");
	t_ShipNames.push_back("Walls/FirstWall");
	t_ShipNames.push_back("Ships/SpazMnik");

	for (int i = 0; i < t_ShipNames.size(); i++)
	{
		m_MeshShips.push_back(InitializeObj(t_ShipNames[i]));
	}
	for (int i = 0; i < 4; i++)
	{
	m_SelectionShips[i].resize( m_MeshShips.size(), 0);
	}


	///init Levels
	m_Levels.resize(p_LevelNames.size(),0);
	//m_MeshLevelWall.resize(p_LevelNames.size(),0);
	//m_MeshLevelWall[0].resize(p_LevelNames.size(),0);
	//m_MeshLevelWall[1].resize(p_LevelNames.size(),0);
	//m_MeshLevelWall[2].resize(p_LevelNames.size(),0);
	for (int i = 0; i < m_Levels.size(); i++)
	{
		m_MeshLevels.push_back(InitializeObj(p_LevelNames[i]));

		std::string t_TempStringForWall;
		//for (int k = 0; k < length; k++) //så vi kan ha flera olika väggar per bana.
		//{
			
		t_TempStringForWall =p_LevelNames[i];
		t_TempStringForWall += "/LevelWalls";//4123
		m_MeshLevelWall.push_back(InitializeObj(t_TempStringForWall));
		/////här e för att fixa in levelskiten men den fackar ur bror vettefan varför kan vara en grej i obj att en skit hade 1 rad med shit men borde inte göra så att skiten fackar ur................................................................................................................................................................................................../////////////m_MeshLevelWall.push_back(InitializeObj(t_TempStringForWall));
		//}

	}

	////skapar object för att rita ut för levelselection
	m_Levels.resize(m_MeshLevels.size(),0);
	m_LevelWalls.resize(m_MeshLevels.size(),0);
	for (int i = 0; i < m_MeshLevels.size(); i++)
	{
		CreateDrawObject(m_MeshLevels[i],XMMatrixIdentity(),XMFLOAT3(1,1,1),m_Levels[i],false);
		CreateDrawObject(m_MeshLevelWall[i],XMMatrixIdentity(),XMFLOAT3(1,1,1),m_LevelWalls[i],false);

	}


	///Laddar inte colours att välja på
	m_Colours.push_back(XMFLOAT3(1,1,0));
	m_Colours.push_back(XMFLOAT3(1,0,1));
	m_Colours.push_back(XMFLOAT3(0,1,1));
	m_Colours.push_back(XMFLOAT3(1,0,0));
	m_Colours.push_back(XMFLOAT3(0,0,1));
	m_Colours.push_back(XMFLOAT3(0,1,0));
	m_Colours.push_back(XMFLOAT3(1,1,1));








	//create a camera, just for testing and stuff, think you'll want to create it later with players tho, because the hud wont rely on camera at first

	//(250,0,-100)			0,0,1					0,1,0   
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->CreateCamera(XMFLOAT3(250,0,-100),XMFLOAT3(0,0,1),XMFLOAT3(0,1,0),XM_PIDIV4,p_Width/2,p_Height/2,1.0f,10000, m_CameraID[i]);
	}
	//m_GraphicEngine->CreateCamera(

	//set cameras
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->UseCamera(i,m_CameraID[i]);
	}




	//m_GraphicEngine->MoveCamera(m_CameraID[0], 20,0,0,0,0);
	//m_GraphicEngine->MoveCamera(m_CameraID[1], 50,0,0,0,0);
	//m_GraphicEngine->MoveCamera(m_CameraID[2], 100,0,0,0,0);
	//m_GraphicEngine->SetCamera(m_CameraID[3], XMFLOAT3(0, 0, -120),XMFLOAT3(0, 0, 1),XMFLOAT3(0, 1, 0));

	//////////////JAWS KOD////////////////

	//UINT t_LightID;
	//m_GraphicEngine->CreateStaticLight(XMFLOAT3(0,0,100), XMFLOAT3(1,1,1), 150, t_LightID);

	//UINT t_jawsObjTEMPID;
	//m_GraphicEngine->CreateHudObject(XMFLOAT2(0, 0),XMFLOAT2(0.5, 0.5), 0, 0, t_jawsObjTEMPID);
	//std::vector<UINT> t_Temptemplist;
	//t_Temptemplist.push_back(t_jawsObjTEMPID);

	//UINT t_jawsHUDTEMPTEMP;
	//m_GraphicEngine->CreateHudTemplate(t_Temptemplist, t_jawsHUDTEMPTEMP);

	//std::vector<XMFLOAT2> t_JawsBAROFFSETS;
	//t_JawsBAROFFSETS.push_back(XMFLOAT2(0,0));

	//UINT t_jawsREALHUD;
	//m_GraphicEngine->CreateHudFromTemplate(t_jawsHUDTEMPTEMP,XMFLOAT3(0,1,0), t_JawsBAROFFSETS, t_jawsREALHUD);

	//m_GraphicEngine->UseHud(0, t_jawsREALHUD);
	//m_GraphicEngine->UseHud(1, t_jawsREALHUD);
	//m_GraphicEngine->UseHud(2, t_jawsREALHUD);
	//m_GraphicEngine->UseHud(3, t_jawsREALHUD);

	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 6; j++)
	//	{
	//		
	//		for (int k = 0; k < 6; k++)
	//		{
	//			int r, g, b;
	//			r = rand() % 2;
	//			g = rand() % 2;
	//			b = rand() % 2;

	//			m_GraphicEngine->CreateStaticLight(XMFLOAT3(-50 + 20*i,-50 + 20*j,-50 + 20*k), XMFLOAT3(r,g,b), 20, t_LightID);
	//		}
	//	}
	//}

	SelectVehicle(); // ska vara där för att initialize selectgrejen
	for (int i = 0; i < 4; i++)
	{
	SetSelectionColour(i, 0);
	}
}

void GraphicHandle::ChangeLevelSelection(int p_WhatLevel)
{

	XMMATRIX t_TempWorldMatrix=XMMatrixIdentity();
	XMFLOAT3 t_TempColour = XMFLOAT3(1,1,1);
	m_GraphicEngine->AddObjectToDrawing(m_Levels[p_WhatLevel]);
	m_GraphicEngine->AddObjectToDrawing(m_LevelWalls[p_WhatLevel]);

	if( m_WhatLevelBefore!=p_WhatLevel)
	{
		m_GraphicEngine->RemoveObjectFromDrawing(m_Levels[m_WhatLevelBefore]);
		m_GraphicEngine->RemoveObjectFromDrawing(m_LevelWalls[m_WhatLevelBefore]);
	}
	m_WhatLevelBefore=p_WhatLevel;
}
void GraphicHandle::UpdateCameraSelectLevel(int p_WhatLevel,int p_TheRotation)
{
	
}
void GraphicHandle::UpdatePlayer(int p_playerID,CXMMATRIX p_PlayerMatrix,CXMMATRIX p_CameraMatrix)
{
	m_GraphicEngine->MoveObject(m_Player[p_playerID], p_PlayerMatrix);
	JohnSetCamera(p_CameraMatrix,p_playerID);//ska vänta 180grader o backa lite med den!!!!
	//uppdatera spelarens mätare cooldownbars(HUD)
}
void GraphicHandle::UpdateSelectVehicle(float p_DeltaTime, int p_PlayerID)
{
	XMMATRIX t_Rotii = XMMatrixRotationY(p_DeltaTime);

	for (int i = 0; i < m_SelectionShipMatrix[p_PlayerID].size(); i++)
	{
		XMMATRIX t_Tempii = XMMatrixMultiply(t_Rotii,XMLoadFloat4x4(&m_SelectionShipMatrix[p_PlayerID][i]));
		m_GraphicEngine->MoveObject(m_SelectionShips[p_PlayerID][i],t_Tempii);
		XMFLOAT4X4 t_Storii;
		XMStoreFloat4x4(&t_Storii,t_Tempii);
		m_SelectionShipMatrix[p_PlayerID][i] = t_Storii;
	}

}

void GraphicHandle::UpdateCamera(UINT p_CameraLogicID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY)
{
	if (p_CameraLogicID < 5)
	{
		m_GraphicEngine->MoveCamera(m_CameraID[p_CameraLogicID],p_Walk,p_Strafe,p_Hover,p_Pitch,p_RotateY);
	}
}
void GraphicHandle::UpdateCameraVehicleSelection(UINT p_CameraLogicID, float p_LookingAtWhatVehicle)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,40);
		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*p_LookingAtWhatVehicle);
		XMMATRIX t_Rot2 = XMMatrixRotationX(-XM_PIDIV4/10);

		t_Rot = XMMatrixMultiply(t_Rot,t_Rot2);//sätter ihop rotationerna
		t_Rot = XMMatrixMultiply(t_Rot, t_Tempura);//roterar matrisen

		t_Tempura = XMMatrixRotationY(XM_PI);//vänder med 180 grader

		t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);//lägger in den sista rotationen


		//t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);

		m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],t_Rot);
		/////////////////////////////////////////////////////////////////////////////////

	}
}
void GraphicHandle::UpdateCameraVehicleSelectionSeperate(UINT p_CameraLogicID, float p_LookingAtWhatVehicle)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,100);
		XMMATRIX t_TempRotationToGetOutCircle = XMMatrixRotationY(XM_PIDIV2*p_CameraLogicID);


		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*p_LookingAtWhatVehicle);
		XMMATRIX t_Rot2 = XMMatrixRotationX(-XM_PIDIV4/10);

		
		t_Tempura = XMMatrixMultiply(t_Tempura,t_TempRotationToGetOutCircle);

		t_Rot = XMMatrixMultiply(t_Rot,t_Rot2);//sätter ihop rotationerna
		t_Rot = XMMatrixMultiply(t_Rot, t_Tempura);//roterar matrisen

		t_Tempura = XMMatrixRotationY(XM_PI);//vänder med 180 grader

		t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);//lägger in den sista rotationen


		//t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);

		m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],t_Rot);
		/////////////////////////////////////////////////////////////////////////////////

	}
}
void GraphicHandle::SetCameraVehicleSelection(UINT p_CameraLogicID)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		//XMMATRIX t_Tempura = XMMatrixTranslation(0,1,25*m_MeshShips.size());
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,100);
		//XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size());
		XMMATRIX t_Rot = XMMatrixRotationY(XM_PIDIV2*p_CameraLogicID*3);
		XMMATRIX t_Rot2 = XMMatrixRotationX(-XM_PIDIV4/4);

		t_Rot = XMMatrixMultiply(t_Rot,t_Rot2);//sätter ihop rotationerna
		t_Rot = XMMatrixMultiply(t_Rot, t_Tempura);//roterar matrisen

		t_Tempura = XMMatrixRotationY(XM_PI);//vänder med 180 grader

		t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);//lägger in den sista rotationen


		//t_Rot = XMMatrixMultiply(t_Tempura,t_Rot);

		m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],t_Rot);
		/////////////////////////////////////////////////////////////////////////////////

	}
}
void GraphicHandle::JohnSetCamera(CXMMATRIX p_World, UINT p_CameraLogicID)
{
	m_GraphicEngine->SetCamera(m_CameraID[p_CameraLogicID],p_World);
}
void GraphicHandle::CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID)
{

	//object id o camera id kan vara samma
	//m_GraphicEngine->CreateDrawObject(p_DrawPieceIDs, p_World,  addToDrawNow,  o_ObjectID);
	m_GraphicEngine->CreateCamera(p_Pos, p_At, p_Up, p_FieldOfView, p_Width, p_Height, p_NearZ, p_FarZ, o_CameraID);
	//m_GraphicEngine->CreatehudObject();//itne klar i engine
	//m_GraphicEngine->CreateParticleSystem();//ej klar
}

void GraphicHandle::DrawGame(float p_DeltaTime) //test 
{
	m_GraphicEngine->DrawGame(p_DeltaTime);
}
void GraphicHandle::CreateLight(int p_PlayerIndex,XMFLOAT3 p_Color,UINT p_ObjectId, LightStruct &p_LightStruct)
{
	m_GraphicEngine->AddObjectLight(p_ObjectId,p_LightStruct.m_Position,p_LightStruct.m_Color,p_LightStruct.m_Radius,p_LightStruct.m_LightID);
}

void GraphicHandle::CreateShipForGame(std::vector<XMMATRIX> p_PlayerWorld)
{
	for (int i = 0; i < m_Player.size(); i++)
	{
		m_GraphicEngine->CreateDrawObject(m_MeshShips[m_PlayerVehicle[i]],
			p_PlayerWorld[i],
			m_Colours[m_PlayerColour[i]],true, 
			m_Player[i]);
		//JAWS KOD
		UINT t_LightID;
		//m_GraphicEngine->AddObjectLight(m_Player[i], XMFLOAT3(0.0f,-0.1f,0.0f), m_Colours[m_PlayerColour[i]],2.0f,t_LightID);
		m_GraphicEngine->AddObjectLight(m_Player[i], XMFLOAT3(0.0f,-0.3f,1.0f), m_Colours[m_PlayerColour[i]],4.0f,t_LightID);
		//m_GraphicEngine->AddObjectLight(m_Player[i], XMFLOAT3(0.0f,-0.1f,2.0f), m_Colours[m_PlayerColour[i]],2.0f,t_LightID);
		/*
		m_GraphicEngine->AddObjectLight(m_Player[i], XMFLOAT3(0.0f,-0.1f,3.0f), m_Colours[m_PlayerColour[i]],2.0f,t_LightID);
		m_GraphicEngine->AddObjectLight(m_Player[i], XMFLOAT3(0.0f,-0.1f,4.0f), m_Colours[m_PlayerColour[i]],2.0f,t_LightID);*/

		//LightStruct t_LightStruct;
		//t_LightStruct.m_Color=m_Colours[m_PlayerColour[i]];//vi skcikar in en färg men kräver att dne har färg i lightstruct
		//t_LightStruct.m_LightID=m_PlayerLight[i];//samma här xD
		//t_LightStruct.m_Position=XMFLOAT3(//ta varje startnissesposition o bajsa lite under dem
		//CreateLight(m_Player[i],m_Colours[m_PlayerColour[i]],m_PlayerLight[i],);//
		//JAWS TEST YES MEGA TEST YES YSE MHHHYEEEEZZZ

		//create a particle system for engines , techis make this sum fun func
		std::vector<Particle> t_InitParticles;
		t_InitParticles.push_back(Particle(XMFLOAT3(0,0,0),XMFLOAT3(0,0,0),XMFLOAT2(1,1),0.0f,100.0f,1));

		UINT t_InitParticleID1, t_InitParticleID2;
		m_GraphicEngine->CreateInitParticleBuffer(t_InitParticles, t_InitParticleID1);
		m_GraphicEngine->CreateInitParticleBuffer(t_InitParticles, t_InitParticleID2);

		UINT t_ParticleBufferDataID1, t_ParticleBufferDataID2;
		//the position here will be updated if object is updated
		m_GraphicEngine->CreateParticleCBSetup(XMFLOAT3(0,0,0), 1, XMFLOAT3(0,0,0), 5, 2.0f, XMFLOAT2(0.2f,0.2f), 0.05f, t_ParticleBufferDataID1);
		m_GraphicEngine->CreateParticleCBSetup(XMFLOAT3(0,0,0), 1, XMFLOAT3(0,0,0), 5, 2.0f, XMFLOAT2(0.2f,0.2f), 0.05f, t_ParticleBufferDataID2);

		UINT t_ParticleSystemID;
		m_GraphicEngine->CreateParticleSystem(0, L"ParticleSmoke.dds",t_InitParticleID1, XMFLOAT3(0.5f,0.3f,0), t_ParticleBufferDataID1, 10000, t_ParticleSystemID );
		m_GraphicEngine->AddObjectParticleSystem(m_Player[i], t_ParticleSystemID);

		m_GraphicEngine->CreateParticleSystem(0, L"ParticleSmoke.dds",t_InitParticleID2, XMFLOAT3(-0.5f,0.3f,0), t_ParticleBufferDataID2, 10000, t_ParticleSystemID );
		m_GraphicEngine->AddObjectParticleSystem(m_Player[i], t_ParticleSystemID);
		
	}
}
void GraphicHandle::SelectVehicle()
{

	XMMATRIX t_WorldMat = XMMatrixTranslation(0,0,30);
	XMFLOAT3 t_Color = XMFLOAT3(1,1,1);
	
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < m_MeshShips.size(); i++)
		{

			XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/4*k);

			t_Rot = XMMatrixMultiply(t_WorldMat, t_Rot);
			

			XMMATRIX t_OffSetTheCircleMat = XMMatrixTranslation(0,0,4);
			XMMATRIX t_OffSetTheCircleMatRotation = XMMatrixRotationY((2*XM_PI*i/m_MeshShips.size()));
			XMMATRIX t_SafteyMeasureMat = XMMatrixMultiply(t_OffSetTheCircleMat,t_OffSetTheCircleMatRotation);

			t_Rot = XMMatrixMultiply( t_SafteyMeasureMat,t_Rot);
			

			CreateDrawObject(m_MeshShips[i],
				t_Rot,
				t_Color, 
				m_SelectionShips[k][i],false);

			XMFLOAT4X4 t_Tempus;
			XMStoreFloat4x4(&t_Tempus, t_Rot);
			m_SelectionShipMatrix[k].push_back(t_Tempus);
		}

	}

}
int GraphicHandle::GetAmountOfVehicles()
{
	return m_MeshShips.size();
}
int GraphicHandle::GetAmountOfLevels()
{
	return m_MeshLevels.size();
}
int GraphicHandle::GetAmountOfColours()
{
	return m_Colours.size();
}
void  GraphicHandle::SetSelectionColour(int p_WhatPlayer, int p_ColourSelected)
{
	for (int i = 0; i < m_MeshShips.size(); i++)
	{		
		m_GraphicEngine->UpdateDrawObjectColor(m_SelectionShips[p_WhatPlayer][i],m_Colours[p_ColourSelected]);
	}
}

void GraphicHandle::Cleanup()
{
	m_GraphicEngine->Cleanup();
}

void GraphicHandle::SetFullScreen(bool p_IsFullScreen)
{
	m_GraphicEngine->SetFullscreenState(p_IsFullScreen);
}

void GraphicHandle::SetColourAndVehicle(std::vector<UINT> p_PlayerColour,std::vector<UINT> p_PlayerVehicle)
{
	for (int i = 0; i < m_Player.size(); i++)
	{
		//m_PlayerVehicle.push_back(p_PlayerVehicle[i]);
		m_PlayerVehicle.at(i) = p_PlayerVehicle[i];
		m_PlayerColour.at(i) = p_PlayerColour[i];
		//m_PlayerColour.push_back(p_PlayerColour[i]);
	}
}

void GraphicHandle::ChangeTexture(UINT p_HUDIDObj)
{
	//byta texture tillgrabben
}
void GraphicHandle::SetAmountOfPlayers(int p_NrOfPlayers)
{
	m_GraphicEngine->SetViewportAmount(p_NrOfPlayers);//not fixed yet, just for testing
	if(m_Player.size()!=p_NrOfPlayers)
	{
		m_Player.clear();	
		m_Player.resize(p_NrOfPlayers,0);
		m_PlayerVehicle.resize(p_NrOfPlayers, 0);
		m_PlayerColour.resize(p_NrOfPlayers,0);
		m_PlayerLight.resize(p_NrOfPlayers,0);
	}
}
//void GraphicHandle::InitializeShip(std::string p_ShipStringName, UINT p_TextureDiffuseSpec, UINT p_TextureNormGlow)
//{
//	std::vector<UINT> t_ObjTemp;
//	t_ObjTemp.clear();
//	m_GraphicEngine->LoadMesh(p_ShipStringName,t_ObjTemp);
//	m_MeshShips.push_back(t_ObjTemp);	
//	for (int i = 0; i < t_ObjTemp.size(); i++)
//	{
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureDiffuseSpec, GraphicEngine::TextureType::DIFFUSE);
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureNormGlow, GraphicEngine::TextureType::NORMAL);
//	}
//
//}
//void GraphicHandle::InitializeLevel(std::string p_LevelStringName, UINT p_Texture,UINT p_TextureNormGlow)
//{
//	std::vector<UINT> t_ObjTemp;
//	t_ObjTemp.clear();
//	m_GraphicEngine->LoadMesh(p_LevelStringName,t_ObjTemp);
//	m_MeshLevels.push_back(t_ObjTemp);	
//	for (int i = 0; i < t_ObjTemp.size(); i++)
//	{
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_Texture,GraphicEngine::TextureType::DIFFUSE);
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureNormGlow, GraphicEngine::TextureType::NORMAL);
//	}
//}
//void GraphicHandle::InitializeWall(std::string p_PlayerWallStringName, UINT p_Texture,UINT p_TextureNormGlow)
//{
//	std::vector<UINT> t_ObjTemp;
//	t_ObjTemp.clear();
//	m_GraphicEngine->LoadMesh(p_PlayerWallStringName,t_ObjTemp);
//	m_MeshPlayerWall.push_back(t_ObjTemp);	
//	for (int i = 0; i < t_ObjTemp.size(); i++)
//	{
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_Texture,GraphicEngine::TextureType::DIFFUSE);
//		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureNormGlow, GraphicEngine::TextureType::NORMAL);
//	}
//}

std::vector <UINT> GraphicHandle::InitializeObj(std::string p_ObjectStringName)
{
	std::string t_TemplateString =p_ObjectStringName;


	UINT t_TempurTextur;
	std::string t_TextString = t_TemplateString;
	t_TextString += "/Texture.dds";
	std::wstring t_LoadTextString = std::wstring(t_TextString.begin(),t_TextString.end());
	m_GraphicEngine->LoadTexture(t_LoadTextString.c_str(), t_TempurTextur);


	UINT t_TempurTexturNG;
	std::string t_TextNGString =t_TemplateString;
	t_TextNGString +="/NG.dds";
	std::wstring t_LoadTextNGString = std::wstring(t_TextNGString.begin(),t_TextNGString.end());
	m_GraphicEngine->LoadTexture(t_LoadTextNGString.c_str(), t_TempurTexturNG);

	std::vector<UINT> r_Mesh;
	r_Mesh.clear();
	std::string t_TempurMeshString =t_TemplateString;
	t_TempurMeshString +="/Mesh.obj";
	m_GraphicEngine->LoadMesh(t_TempurMeshString,r_Mesh);
	

	for (int i = 0; i < r_Mesh.size(); i++)
	{
		m_GraphicEngine->AddTextureToDrawPiece(r_Mesh[0],t_TempurTextur, GraphicEngine::TextureType::DIFFUSE);
		m_GraphicEngine->AddTextureToDrawPiece(r_Mesh[0],t_TempurTexturNG, GraphicEngine::TextureType::NORMAL);
	}
	return r_Mesh;
}

UINT GraphicHandle::CreateWall(int p_WhatWall,CXMMATRIX p_PlayerWallWorld,int p_WhatPlayer)
{
	UINT r_WhatWall;


	CreateDrawObject(m_MeshPlayerWall[p_WhatWall],
		p_PlayerWallWorld,
		m_Colours[m_PlayerColour[p_WhatPlayer]], 
		r_WhatWall,true);

	//LightStruct t_LightStruct;
	//t_LightStruct.m_Color=m_Colours[m_PlayerColour[i]];//vi skcikar in en färg men kräver att dne har färg i lightstruct
	//t_LightStruct.m_LightID=m_PlayerLight[i];//samma här xD
	//t_LightStruct.m_Position=XMFLOAT3(//ta varje startnissesposition o bajsa lite under dem
	//CreateLight(m_Player[i],m_Colours[m_PlayerColour[i]],m_PlayerLight[i],);//

	return r_WhatWall;

}


void GraphicHandle::CreateDrawObject(std::vector <UINT> p_UINTMeshLista, CXMMATRIX p_World,XMFLOAT3 p_Colour,UINT & o_ObjectID, bool p_ShouldItBeDrawn)
{
	m_GraphicEngine->CreateDrawObject(p_UINTMeshLista,p_World,p_Colour,p_ShouldItBeDrawn, o_ObjectID);
}

void GraphicHandle::RemoveLevelDraw(int p_RemoveLevelDraw)
{
	m_GraphicEngine->RemoveObjectFromDrawing(m_Levels[p_RemoveLevelDraw]);
	m_GraphicEngine->RemoveObjectFromDrawing(m_LevelWalls[p_RemoveLevelDraw]);
	//m_GraphicEngine->AddObjectToDrawing();//ta bort skit ffs. levelväggarna
}
void GraphicHandle::AddLevelDraw(int p_AddLevelDraw)
{
	m_GraphicEngine->AddObjectToDrawing(m_Levels[p_AddLevelDraw]);
	m_GraphicEngine->AddObjectToDrawing(m_LevelWalls[p_AddLevelDraw]);
	//m_GraphicEngine->AddObjectToDrawing(m_Mesh);//ta bort skit ffs. levelväggarna o lägg till här iaf
}
void GraphicHandle::RemoveSelectionDraw()
{
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < m_SelectionShips[k].size(); i++)
		{
			m_GraphicEngine->RemoveObjectFromDrawing(m_SelectionShips[k][i]);
		}
	}

}
void GraphicHandle::AddSelectionDraw()
{
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < m_SelectionShips[k].size(); i++)
		{
			m_GraphicEngine->AddObjectToDrawing(m_SelectionShips[k][i]);
		}
	}

}
void GraphicHandle::LoadTexture(const wchar_t* p_FileName, UINT &o_TextureID)
{
	m_GraphicEngine->LoadTexture(p_FileName, o_TextureID);
}

void GraphicHandle::SetViewportAmount(UINT p_ViewportAmount)
{
	m_GraphicEngine->SetViewportAmount(p_ViewportAmount);
}

void GraphicHandle::RemoveObject(UINT p_RemoveAt)
{
	m_GraphicEngine->RemoveObject(p_RemoveAt);
}
void GraphicHandle::RemovePlayers()
{
	for (int i = 0; i < m_Player.size(); i++)
	{
		RemoveObject(m_Player[i]);
	}
}
//light functions

void GraphicHandle::CreateMapLights(std::vector<XMFLOAT3> p_CenterSpline)
{
	int full = p_CenterSpline.size();
	int counter = 0;
	float r = 1;
	float g = 0;
	float b = 0;
	int colChanger = 0;
	float t_ColChanger =  0.05f;
	while (counter < (full))
	{
		UINT id;
		m_GraphicEngine->CreateStaticLight(p_CenterSpline[counter],XMFLOAT3(r,g,b), 15.0f,id);


		if (colChanger == 0) //här blir det gult
		{
			g += t_ColChanger;
			if (g >= 1)
			{
				colChanger = 1;
				g = 1;
			}
		}
		else if (colChanger == 1) // här blir det grönt
		{
			r -= t_ColChanger;
			if (r <= 0)
			{
				colChanger = 2;
				r = 0;
			}
		}
		else if (colChanger == 2) // här blir det turkås
		{
			b += t_ColChanger;
			if (b >= 1)
			{
				colChanger = 3;
				b = 1;
			}
		}
		else if (colChanger == 3) //här blir det blått
		{
			g -= t_ColChanger;
			if (g <= 0)
			{
				colChanger = 4;
				g = 0;
			}
		}
		else if (colChanger == 4) //här blir det lila
		{
			r += t_ColChanger;
			if (r >= 1)
			{
				colChanger = 5;
				r = 1;
			}
		}
		else if (colChanger == 5) //här blir det rött
		{
			b -= t_ColChanger;
			if (b <= 0)
			{
				colChanger = 0;
				b = 0;
			}
		}


		counter += 5;
	}
}



//hud functions


void GraphicHandle::CreateHUDObject(XMFLOAT2 p_Position, XMFLOAT2 p_Offset, std::vector<UINT> p_textureIDs, UINT &o_HudObjectID)
{
	m_GraphicEngine->CreateHudObject(p_Position, p_Offset, p_textureIDs, o_HudObjectID);
}

void GraphicHandle::CreateHudTemplate(std::vector<UINT> p_HudObjects, UINT& o_TemplateID)
{
	m_GraphicEngine->CreateHudTemplate(p_HudObjects, o_TemplateID);
}

void GraphicHandle::CreateHudFromTemplate(UINT p_HudTemplate, int p_Color, std::vector<XMFLOAT2> p_BarOffsets, UINT &o_HudID)
{
	m_GraphicEngine->CreateHudFromTemplate(p_HudTemplate, m_Colours[p_Color], p_BarOffsets, o_HudID);
}

void GraphicHandle::UseHud(UINT p_ViewportID, UINT p_HudID)
{
	m_GraphicEngine->UseHud(p_ViewportID, p_HudID);
}

void GraphicHandle::ChangeHudObjectTexture(UINT p_hudID, UINT p_objectID, UINT p_useFrontTexture)
{
	m_GraphicEngine->ChangeTextureOnHudObject(p_hudID,p_objectID,p_useFrontTexture);
}

void GraphicHandle::UpdateHudBarOffset(UINT p_hudID, UINT p_objectID, DirectX::XMFLOAT2 p_barOffset)
{
	m_GraphicEngine->UpdateBarOffset(p_hudID,p_objectID,p_barOffset);
}
