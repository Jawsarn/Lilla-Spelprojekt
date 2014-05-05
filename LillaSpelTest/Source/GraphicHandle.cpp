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

	////initWallTextures
	m_GraphicEngine->LoadTexture(L"Wall.dds",t_TempurTextur);
	m_PlayerWallTexture.push_back(t_TempurTextur);


	////Init alla skepp
	m_GraphicEngine->LoadTexture(L"spaceship.dds", t_TempurTextur);
	m_ShipTexture.push_back(t_TempurTextur);
	m_GraphicEngine->LoadTexture(L"spaceship1.dds", t_TempurTextur);
	m_ShipTexture.push_back(t_TempurTextur);
	m_GraphicEngine->LoadTexture(L"spaceship2.dds", t_TempurTextur);
	m_ShipTexture.push_back(t_TempurTextur);

	m_GraphicEngine->LoadTexture(L"spaceshipNG.dds", t_TempurTextur);
	m_ShipTexture.push_back(t_TempurTextur);

	
	InitializeShip("spaceship.obj",m_ShipTexture[0], m_ShipTexture[3] ); //normalGlow texture is same for all ship, fix to 4123
	InitializeShip("spaceship1.obj",m_ShipTexture[1], m_ShipTexture[3] );
	InitializeShip("spaceship2.obj",m_ShipTexture[2],m_ShipTexture[3] );

	m_SelectionShips.resize( m_MeshShips.size(), 0);




	///init Levels
	m_Levels.resize(p_LevelNames.size(),0);
	for (int i = 0; i < p_LevelNames.size(); i++)
	{
		std::string t_stTompa =p_LevelNames[i];
		t_stTompa +=".dds";
		std::wstring t_sTompa = std::wstring(t_stTompa.begin(),t_stTompa.end());
		m_GraphicEngine->LoadTexture(t_sTompa.c_str(),t_TempurTextur);

		std::string t_Tompa =p_LevelNames[i];
		t_Tompa += ".obj";
		InitializeLevel(t_Tompa,t_TempurTextur);
	}

	m_Levels.resize(m_MeshLevels.size(),0);
	for (int i = 0; i < m_MeshLevels.size(); i++)
	{
		CreateDrawObject(m_MeshLevels[i],XMMatrixIdentity(),XMFLOAT3(1,1,1),m_Levels[i],false);
	}

	//CreateDrawObject(m_MeshLevels[0],XMMatrixIdentity(),XMFLOAT3(1,1,1),m_Levels[0],true);

	////init walls
	InitializeWall("wall.obj",m_PlayerWallTexture[0]);





	///Laddar inte colours att välja på
	m_Colours.push_back(XMFLOAT3(1,1,0));
	m_Colours.push_back(XMFLOAT3(1,0,1));
	m_Colours.push_back(XMFLOAT3(0,1,1));
	m_Colours.push_back(XMFLOAT3(1,0,0));
	m_Colours.push_back(XMFLOAT3(0,0,1));
	m_Colours.push_back(XMFLOAT3(0,1,0));
	m_Colours.push_back(XMFLOAT3(1,1,1));
	//creating a ship that the player is going to use, move to other place when we've done testing




	//////////////////////////////////////////////////////////////TEST!!!!!!!!!!!!!!!!!!!///////////////////////////
	XMMATRIX t_Mat = XMMatrixTranslation(0,0,0)* XMMatrixScaling(1,1,1);
	XMMATRIX t_World=t_Mat;
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
	//t_Color = XMFLOAT3(1,0,1);
	//plajercullur.push_back(t_Color);
	t_Mat = XMMatrixTranslation(0,0,5); 
	plajerwurld.push_back(t_Mat);

	//t_Mat = XMMatrixTranslation(0,0,0);
	//plajerwurld.push_back(t_Mat);
	//t_Mat = XMMatrixTranslation(20,20,20);
	//plajerwurld.push_back(t_Mat);
	//t_Mat = XMMatrixTranslation(30,30,30);
	//plajerwurld.push_back(t_Mat);
	//t_Mat = XMMatrixTranslation(40,40,40);
	//plajerwurld.push_back(t_Mat);

	//VilkenVehicle.push_back(0);

	//VilkenVehicle.push_back(0);
	//VilkenVehicle.push_back(0);
	//VilkenVehicle.push_back(0);
	//t_Mat = XMMatrixTranslation(10,10,10); 
	//StartGame(0,VilkenVehicle,plajerwurld,plajercullur,t_World,t_Color);




	//test 
	//UINT t_temptest;
	//t_temptest=CreateWall(0,plajerwurld[0],0);
	//m_GraphicEngine->RemoveObjectFromDrawing(t_temptest);
	//m_GraphicEngine->AddObjectToDrawing(t_temptest);
	//SelectVehicle();






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

	SelectVehicle();

	//	UpdatePlayer(0,t_Mat);
}

void GraphicHandle::ChangeLevelSelection(int p_WhatLevel)
{

	XMMATRIX t_TempWorldMatrix=XMMatrixIdentity();
	XMFLOAT3 t_TempColour = XMFLOAT3(1,1,1);
	m_GraphicEngine->AddObjectToDrawing(m_Levels[p_WhatLevel]);

	if( m_WhatLevelBefore!=p_WhatLevel)
	{
		m_GraphicEngine->RemoveObjectFromDrawing(m_Levels[m_WhatLevelBefore]);
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
void GraphicHandle::UpdateCameraVehicleSelection(UINT p_CameraLogicID, float p_LookingAtWhatVehicle)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,8*m_MeshShips.size());
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
void GraphicHandle::SetCameraVehicleSelection(UINT p_CameraLogicID)
{
	if (p_CameraLogicID < 5)
	{	
		/////////////////////////////////fungerande
		XMMATRIX t_Tempura = XMMatrixTranslation(0,1,25*m_MeshShips.size());
		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size());
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

void GraphicHandle::DrawGame() //test 
{
	m_GraphicEngine->DrawGame();
}
void GraphicHandle::CreateLight(int p_PlayerIndex,XMFLOAT3 p_Color,UINT p_ObjectId, LightStruct &p_LightStruct)
{
	m_GraphicEngine->AddObjectLight(p_ObjectId,p_LightStruct.m_Position,p_LightStruct.m_Color,p_LightStruct.m_Radius,p_LightStruct.m_LightID);
}

void GraphicHandle::CreateShipForGame( std::vector<int> p_WhatVehicle,
									  std::vector<XMMATRIX> p_PlayerWorld,
									  std::vector<XMFLOAT3>p_Color,
									  CXMMATRIX p_LevelWorld,
									  XMFLOAT3 p_LevelColor)
{
	for (int i = 0; i < m_Player.size(); i++)
	{
		m_GraphicEngine->CreateDrawObject(m_MeshShips[p_WhatVehicle[i]],
			p_PlayerWorld[i],
			p_Color[i],true, 
			m_Player[i]);

		//LightStruct t_LightStruct;
		//t_LightStruct.m_Color=m_Colours[m_PlayerColour[i]];//vi skcikar in en färg men kräver att dne har färg i lightstruct
		//t_LightStruct.m_LightID=m_PlayerLight[i];//samma här xD
		//t_LightStruct.m_Position=XMFLOAT3(//ta varje startnissesposition o bajsa lite under dem
		//CreateLight(m_Player[i],m_Colours[m_PlayerColour[i]],m_PlayerLight[i],);//
	}

	//m_GraphicEngine->CreateDrawObject(m_MeshLevels[p_WhatLevel],p_LevelWorld,p_Color[0],true,m_CurrentLevel);
}
void GraphicHandle::SelectVehicle()
{
	XMMATRIX t_WorldMat = XMMatrixTranslation(0,0,5*m_MeshShips.size());

	XMFLOAT3 t_Color = XMFLOAT3(0,1,0);

	for (int i = 0; i < m_MeshShips.size(); i++)
	{
		XMMATRIX t_Rot = XMMatrixRotationY(2*XM_PI/m_MeshShips.size()*i);

		t_Rot = XMMatrixMultiply(t_WorldMat, t_Rot);
		CreateDrawObject(m_MeshShips[i],
			t_Rot,
			t_Color, 
			m_SelectionShips[i],false);

		//m_GraphicEngine->CreateDrawObject(m_MeshShips[i],
		//t_Rot,
		//t_Color,
		//true, 
		//m_SelectionShips[i]);

		XMFLOAT4X4 t_Tempus;
		XMStoreFloat4x4(&t_Tempus, t_Rot);
		m_SelectionShipMatrix.push_back(t_Tempus);
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
	m_Player=p_PlayerVehicle;
	m_PlayerColour=p_PlayerColour;
}

void GraphicHandle::ChangeTexture(UINT p_HUDIDObj)
{
	//byta texture tillgrabben
}
void GraphicHandle::SetAmountOfPlayers(int p_NrOfPlayers)
{
	if(m_Player.size()!=p_NrOfPlayers)
	{
		m_Player.clear();	
		m_GraphicEngine->SetViewportAmount(p_NrOfPlayers);//not fixed yet, just for testing
		m_Player.resize(p_NrOfPlayers,0);
		m_PlayerColour.resize(p_NrOfPlayers,0);
		m_PlayerLight.resize(p_NrOfPlayers,0);
	}
}
void GraphicHandle::InitializeShip(std::string p_ShipStringName, UINT p_TextureDiffuseSpec, UINT p_TextureNormGlow)
{
	std::vector<UINT> t_ObjTemp;
	t_ObjTemp.clear();
	m_GraphicEngine->LoadMesh(p_ShipStringName,t_ObjTemp);
	m_MeshShips.push_back(t_ObjTemp);	
	for (int i = 0; i < t_ObjTemp.size(); i++)
	{
		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureDiffuseSpec, GraphicEngine::TextureType::DIFFUSE);
		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_TextureNormGlow, GraphicEngine::TextureType::NORMAL);
	}
}
void GraphicHandle::InitializeLevel(std::string p_LevelStringName, UINT p_Texture)
{
	std::vector<UINT> t_ObjTemp;
	t_ObjTemp.clear();
	m_GraphicEngine->LoadMesh(p_LevelStringName,t_ObjTemp);
	m_MeshLevels.push_back(t_ObjTemp);	
	for (int i = 0; i < t_ObjTemp.size(); i++)
	{
		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_Texture,GraphicEngine::TextureType::DIFFUSE);
	}
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
void GraphicHandle::InitializeWall(std::string p_PlayerWallStringName, UINT p_Texture)
{
	std::vector<UINT> t_ObjTemp;
	t_ObjTemp.clear();
	m_GraphicEngine->LoadMesh(p_PlayerWallStringName,t_ObjTemp);
	m_MeshPlayerWall.push_back(t_ObjTemp);	
	for (int i = 0; i < t_ObjTemp.size(); i++)
	{
		m_GraphicEngine->AddTextureToDrawPiece(t_ObjTemp[0],p_Texture,GraphicEngine::TextureType::DIFFUSE);
	}
}

void GraphicHandle::CreateDrawObject(std::vector <UINT> p_UINTMeshLista, CXMMATRIX p_World,XMFLOAT3 p_Colour,UINT & o_ObjectID, bool p_ShouldItBeDrawn)
{
	m_GraphicEngine->CreateDrawObject(p_UINTMeshLista,p_World,p_Colour,p_ShouldItBeDrawn, o_ObjectID);
}

void GraphicHandle::RemoveLevelDraw(int p_RemoveLevelDraw)
{
	m_GraphicEngine->RemoveObjectFromDrawing(m_Levels[p_RemoveLevelDraw]);
}
void GraphicHandle::AddLevelDraw(int p_AddLevelDraw)
{
	m_GraphicEngine->AddObjectToDrawing(m_Levels[p_AddLevelDraw]);
}
void GraphicHandle::RemoveSelectionDraw()
{
	for (int i = 0; i < m_SelectionShips.size(); i++)
	{
		m_GraphicEngine->RemoveObjectFromDrawing(m_SelectionShips[i]);
	}
}
void GraphicHandle::AddSelectionDraw()
{
	for (int i = 0; i < m_SelectionShips.size(); i++)
	{
		m_GraphicEngine->AddObjectToDrawing(m_SelectionShips[i]);
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



//hud functions


void GraphicHandle::CreateHUDObject(XMFLOAT2 p_Position, XMFLOAT2 p_Offset, int p_TextureID1, int p_TextureID2, UINT &o_HudObjectID)
{
	m_GraphicEngine->CreateHudObject(p_Position, p_Offset, p_TextureID1 , p_TextureID2, o_HudObjectID);
}

void GraphicHandle::CreateHudTemplate(std::vector<UINT> p_HudObjects, UINT& o_TemplateID)
{
	m_GraphicEngine->CreateHudTemplate(p_HudObjects, o_TemplateID);
}

void GraphicHandle::CreateHudFromTemplate(UINT p_HudTemplate, XMFLOAT3 p_Color, std::vector<XMFLOAT2> p_BarOffsets, UINT &o_HudID)
{
	m_GraphicEngine->CreateHudFromTemplate(p_HudTemplate, p_Color, p_BarOffsets, o_HudID);
}

void GraphicHandle::UseHud(UINT p_ViewportID, UINT p_HudID)
{
	m_GraphicEngine->UseHud(p_ViewportID, p_HudID);
}

void GraphicHandle::ChangeHudObjectTexture(UINT p_hudID, UINT p_objectID, bool p_useFrontTexture)
{
	m_GraphicEngine->ChangeTextureOnHudObject(p_hudID,p_objectID,p_useFrontTexture);
}

