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
	m_GraphicEngine->LoadTexture(L"Boll.dds", diffuseBollTestTexture);

	//load a ship mesh 
	std::vector<UINT> t_Ship;
	m_GraphicEngine->LoadMesh("Tube.obj",t_Ship);
	m_Ships.push_back(t_Ship);

	//add a texture to a ship mesh
	m_GraphicEngine->AddTextureToDrawPiece(t_Ship[0],diffuseBollTestTexture,GraphicEngine::TextureType::DIFFUSE);

	//creating a ship that the player is going to use, move to other place when we've done testing
	UINT objectID;
	XMMATRIX t_Mat = XMMatrixIdentity();
	XMFLOAT3 t_Color = XMFLOAT3(0,1,0);
	m_GraphicEngine->CreateDrawObject(m_Ships[0],t_Mat, t_Color,true, objectID);

	//add light to an already existing ship, (note to self maybe, if we have two ships that are the same for two players, same textures will be used, but not same lights, this is ok)
	//second note, add light is in object space
	UINT playerLightOne; //add to a player light array maybe?
	m_GraphicEngine->AddObjectLight(objectID, XMFLOAT3(0,-2,0),XMFLOAT3(1,0,0),3, playerLightOne);


	//create a camera, just for testing and stuff, think you'll want to create it later with players tho, because the hud wont rely on camera at first
	
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->CreateCamera(XMFLOAT3(0,0,-160),XMFLOAT3(0,0,1),XMFLOAT3(0,1,0),XM_PIDIV4,p_Width/2,p_Height/2,1.0f,10000, m_CameraID[i]);
	}
	
	//set cameras
	for (int i = 0; i < 4; i++)
	{
		m_GraphicEngine->UseCamera(i,m_CameraID[i]);
	}

	 //not fixed yet, just for testing
	m_GraphicEngine->SetViewportAmount(4);

	m_GraphicEngine->MoveCamera(m_CameraID[0], 20,0,0,0,0);
	m_GraphicEngine->MoveCamera(m_CameraID[1], 50,0,0,0,0);
	m_GraphicEngine->MoveCamera(m_CameraID[2], 0,0,0,0,0);
	m_GraphicEngine->MoveCamera(m_CameraID[3], 0,0,0,0,0);

}

void GraphicHandle::UpdatePlayer(int p_playerID,CXMMATRIX p_matrix)
{
	m_GraphicEngine->MoveObject(p_playerID, p_matrix);
	//uppdatera spelarens mätare cooldownbars(HUD)

}

void GraphicHandle::UpdateCamera(UINT p_CameraLogicID,float p_Walk, float p_Strafe, float p_Hover, float p_Pitch, float p_RotateY)
{
	if (p_CameraLogicID < 5)
	{
		m_GraphicEngine->MoveCamera(m_CameraID[p_CameraLogicID],p_Walk,p_Strafe,p_Hover,p_Pitch,p_RotateY);
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