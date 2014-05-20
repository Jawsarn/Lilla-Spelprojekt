#pragma once
#include "menuscreen.h"
#include <string>

class JoinGameScreen :
	public MenuScreen
{
private:
	enum PlayerStatus{DISCONNECTED,CHOOSE_MODELL, READY};
	PlayerStatus m_playerStatus[4];
	std::vector<std::string> m_tauntSounds;
	int m_numberOfModells;
	int m_numberOfColors;
	int m_numberOfTaunts;
	int m_modell[4];
	float m_modellIncrease[4];
	int m_color[4];
	int m_selectedTauntSound[4];
	float m_whatVehicleToLookAt[4];
	unsigned int m_hudIDs[4];
public:
	JoinGameScreen(void);
	JoinGameScreen(GameInfo* p_gameInfo, GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager);
	~JoinGameScreen(void);

	int Update(float p_dt,std::vector<UserCMD>* userCMD);
	void Draw();
	void Initialize();

private:
	void SaveInfo();
	void ModellChanger(int i, float p_dt, std::vector<UserCMD>* userCMD);
	void ColorChanger(int i, float p_dt, std::vector<UserCMD>* userCMD);
	void TauntChanger(int i, float p_dt, std::vector<UserCMD>* userCMD);
	void MakeHud(const wchar_t* p_textureName, int p_hudIndex);
	bool ReadyCheck(int p_whoChecked);

};

