#pragma once
#include "menuscreen.h"


class JoinGameScreen :
	public MenuScreen
{
private:
	enum PlayerStatus{DISCONNECTED,CHOOSE_MODELL, CHOOSE_COLOR, READY};
	PlayerStatus m_playerStatus[4];
	int numberOfModells;
	int m_modell[4];
	int m_modellIncrease[4];
	int m_color[4];
public:
	JoinGameScreen(void);
	JoinGameScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle);
	~JoinGameScreen(void);

	int Update(float p_dt,std::vector<UserCMD>* userCMD);
	void Draw();
	void Initialize();

private:
	void SaveInfo();
	void ModellChanger(int i, float p_dt, std::vector<UserCMD>* userCMD);
	void ColorChanger(int i, float p_dt, std::vector<UserCMD>* userCMD);
};

