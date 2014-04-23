#pragma once
#include "menuscreen.h"


class JoinGameScreen :
	public MenuScreen
{
private:
	enum PlayerStatus{DISCONNECTED,CHOOSE_MODELL, CHOOSE_COLOR, READY};
	PlayerStatus m_playerStatus[4];
	int m_modell[4];
	int m_color[4];
public:
	JoinGameScreen(void);
	JoinGameScreen(GameInfo* p_gameInfo);
	~JoinGameScreen(void);

	int Update(std::vector<UserCMD>* userCMD, float p_dt);
	void Draw();

private:
	void SaveInfo();
};

