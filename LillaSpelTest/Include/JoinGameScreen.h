#pragma once
#include "menuscreen.h"

class JoinGameScreen :
	public MenuScreen
{
private:
	enum PlayerStatus{DISCONNECTED,CHOOSE_MODDEL, CHOOSE_COLOR, READY};
	PlayerStatus players[4];
	int model[4];
public:
	JoinGameScreen(void);
	~JoinGameScreen(void);

	int Update(std::vector<UserCMD>* userCMD);
	void Draw();
};

