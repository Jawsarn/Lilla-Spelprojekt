
#include "menuscreen.h"
class GoalScreen :
	public MenuScreen
{
private:
	UINT m_hudHandle;
public:

	GoalScreen(void);
	GoalScreen( GraphicHandle* p_graphicsHandle, AudioManager* p_audioManager);
	~GoalScreen(void);

	int Update(float p_dt, std::vector<UserCMD>* p_userCMDs);
	void Draw();
	void Initialize();
};
