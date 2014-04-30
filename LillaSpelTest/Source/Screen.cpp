#include "Screen.h"
Screen::Screen(void)
{
}
Screen::Screen(GraphicHandle* p_graphicHandle)
{
	m_graphicHandle = p_graphicHandle;
}

Screen::~Screen(void)
{
}

int Screen::Update(float p_dt, std::vector<UserCMD>* p_userCMDS)
{
	return 1;
}

void Screen::Draw()
{

}