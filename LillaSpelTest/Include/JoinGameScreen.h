#pragma once
#include "menuscreen.h"

class JoinGameScreen :
	public MenuScreen
{
private:
	enum PlayerStatus{DISCONNECTED,CHOOSE_MODELL, CHOOSE_COLOR, READY};
	PlayerStatus playerStatus[4];
	int modell[4];
	int color[4];
public:
	JoinGameScreen(void);
	~JoinGameScreen(void);

	int Update(std::vector<UserCMD>* userCMD);
	void Draw();
};

