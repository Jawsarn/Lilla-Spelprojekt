#include "GraphicHandle.h"
#include "GraphicEngine.h"

GraphicHandle::GraphicHandle()
{

}
GraphicHandle::~GraphicHandle()
{

}

void GraphicHandle::Initialize()
{
	m_GraphicEngine = m_GraphicEngine->GetInstance();

	std::vector<UINT> t_Ship;
	m_GraphicEngine->LoadMesh(L"ships.obj",t_Ship);
	m_Ships.push_back(t_Ship);
	m_GraphicEngine->AddTextureToDrawPiece(t_Ship[0],0,GraphicEngine::TextureType::SPECULAR);
	//initialize varje skepp

	

}
void GraphicHandle::Draw()
{

}
void GraphicHandle::UpdatePlayer(int p_playerID,CXMMATRIX p_matrix)
{
	m_GraphicEngine->MoveObject(p_playerID, p_matrix);
	//uppdatera spelarens mätare cooldownbars(HUD)

}
void GraphicHandle::CreatePlayer(std::vector<UINT> p_DrawPieceIDs, CXMMATRIX p_World, bool addToDrawNow, UINT &o_ObjectID,XMFLOAT3 p_Pos, XMFLOAT3 p_At, XMFLOAT3 p_Up, float p_FieldOfView, float p_Width, float p_Height, float p_NearZ, float p_FarZ, UINT &o_CameraID)
{

	//object id o camera id kan vara samma
	m_GraphicEngine->CreateObject(p_DrawPieceIDs, p_World,  addToDrawNow,  o_ObjectID);
	m_GraphicEngine->CreateCamera(p_Pos, p_At, p_Up, p_FieldOfView, p_Width, p_Height, p_NearZ, p_FarZ, o_CameraID);
	m_GraphicEngine->CreatehudObject();//itne klar i engine
	//m_GraphicEngine->CreateParticleSystem();//ej klar
	

}