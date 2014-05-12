
#include <Windows.h>
#include <windowsx.h>
#include "Controller.h"
#include <vector>
#include "UserCMD.h"
#include <DirectXMath.h>
#include "UserCMDHandler.h"
#include "AzookaTest.h"
#include "MysteriskTest.h"
#include "GameScreen.h"
#include "GameSetupScreen.h"
#include "JoinGameScreen.h"
#include "Struct_GameInfo.h"
#include "MainMenuScreen.h"
#include "OptionsScreen.h"
#include "PauseScreen.h"
#include "AudioManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT InitializeWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow);
void Update(std::vector<UserCMD>* p_userCMDs);
void Run();

HINSTANCE	handleInstance;
HWND	m_HandleWindow;

ApplicationState m_state;
std::vector<std::string> m_levelNames;
//// The different screens ////
Screen* m_mainMenuScreen;
Screen* m_gameSetupScreen;
Screen* m_optionsScreen;
Screen* m_joinGameScreen;
PauseScreen* m_pauseScreen;
GameScreen* m_gameScreen;
GameInfo m_gameInfo;

////audiomanager to send to different screens////
AudioManager* m_audioManager;


#include "GraphicHandle.h"

float m_DeltaTime;
float m_GameTime;
ULONGLONG m_PrevTime;
XMFLOAT2 m_LastMousePos;
int m_ActiveCamera = 0; //just for testing and show

GraphicHandle* m_GraphicHandle;

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	InitializeWindow(hInstance,nCmdShow);
	

	m_LastMousePos = XMFLOAT2(0,0);


	m_levelNames.push_back("Levels/Assault");
	m_levelNames.push_back("Levels/Dust2");
	m_levelNames.push_back("Levels/Aztec");
	m_levelNames.push_back("Levels/Aztec2");


	m_GraphicHandle = m_GraphicHandle->GetInstance();
	m_GraphicHandle->Initialize(1920, 1080, m_HandleWindow,m_levelNames); //fix this ipnleagut variables right
	m_GraphicHandle->SetFullScreen(false);
	m_audioManager = m_audioManager->GetInstance();
	m_audioManager->PlaySpecificSound("main.wav",true,false);


	m_mainMenuScreen = new MainMenuScreen(m_GraphicHandle, m_audioManager);
	m_gameSetupScreen = new GameSetupScreen(&m_gameInfo,m_GraphicHandle, m_audioManager);
	m_optionsScreen = new OptionsScreen(&m_gameInfo,m_GraphicHandle, m_audioManager);
	m_joinGameScreen = new JoinGameScreen(&m_gameInfo,m_GraphicHandle, m_audioManager);
	m_pauseScreen = new PauseScreen(&m_gameInfo,m_GraphicHandle, m_audioManager);


	//m_gameScreen = new GameScreen("dust2", 4, m_GraphicHandle);
	m_state = MAIN_MENU_SCREEN;
	Run();

	return 0;
}


void Run() 
{
	
	AzookaTest t_azookaTest = AzookaTest();
	std::vector<UserCMD> *userCMDS = new std::vector<UserCMD>();
	UserCMDHandler userCMDHandler = UserCMDHandler();
	//GameScreen gameScreen = GameScreen("dust2", 4, m_GraphicHandle);
	for (int i = 0; i < 4; i++)
	{
		UserCMD t_userCMD = UserCMD(i);
		userCMDS->push_back(t_userCMD);
	}
	

	MysteriskTest t_Mtest = MysteriskTest(m_GraphicHandle);
	t_Mtest.Run(userCMDS,m_DeltaTime);

	//message game loop
	MSG msg = {0};
	while( WM_QUIT != msg.message )
	{
		//get messages and update
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else  //if there are no messages, update and draw
		{
			m_audioManager->Update();
			for (int i = 0; i < 4; i++) ///Fixes UserCMDs for all connected players
			{
				if (userCMDS->at(i).controller.IsConnected())
				{
					userCMDHandler.AlterUserCMD(userCMDS->at(i));
				} 
				else
				{
					////Player i controller disconnected, plz connect again message
				}
			}
			ULONGLONG timeCur = GetTickCount64();
			if( m_PrevTime == 0 )
				m_PrevTime = timeCur;
			m_DeltaTime = ( timeCur - m_PrevTime ) / 1000.0f;
			m_GameTime += m_DeltaTime;
			m_PrevTime = timeCur;
			Update(userCMDS);
			//t_azookaTest.Run();

			
			//t_Mtest.Run(userCMDS,m_DeltaTime);


		
		
			//gameScreen.Update(m_DeltaTime,userCMDS);
					//m_graphicHandle->JohnSetCamera(m_players[i]->GetWorldMatrix(), i);
		//	XMMATRIX t_debugCameraMatrix = t_azookaTest.GetDebugCameraWorldMatrix(&userCMDS->at(0), m_DeltaTime);
		//	m_GraphicHandle->JohnSetCamera(t_debugCameraMatrix, 2);

			///UPDATE & DRAW TEMPDRAAWWWWW
			//m_GraphicHandle->UpdateSelectVehicle(m_DeltaTime);
			m_GraphicHandle->DrawGame(m_DeltaTime);
		}
	}

	//cleanup
}

void RunInitialization()
{
	
	int t_playerOnline =0;
	switch (m_state)
	{
	case GAME_SETUP_SCREEN:
		m_gameSetupScreen->Initialize();
		break;
	case PAUSE_SCREEN:
		m_pauseScreen->Initialize(m_gameScreen->GetPauseDudeIndex());
		break;
	case GAME_SCREEN:
		delete m_gameScreen;
		for (int i = 0; i < 4; i++)
		{
			if(m_gameInfo.playerOnline[i])
				t_playerOnline++;
		}
		m_gameScreen = new GameScreen(m_gameInfo.playerColor, m_gameInfo.shipModell, m_levelNames[m_gameInfo.map], t_playerOnline, m_GraphicHandle, m_audioManager);
		break;
	case JOIN_GAME_SCREEN:
		m_joinGameScreen->Initialize();
		break;
	case OPTIONS_SCREEN:
		m_optionsScreen->Initialize();
		break;
	case MAIN_MENU_SCREEN:
		m_mainMenuScreen->Initialize();
		break;
	case SHUT_DOWN:
		break;
	default:
		break;
	}
}

void Update(std::vector<UserCMD>* p_userCMDs)
{
	switch (m_state)
	{
	case GAME_SETUP_SCREEN:
		m_state = (ApplicationState)m_gameSetupScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != GAME_SETUP_SCREEN)
		{
			RunInitialization();
		}
		break;
	case PAUSE_SCREEN:
		m_state = (ApplicationState)m_pauseScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != PAUSE_SCREEN )
		{
			if (m_state != GAME_SCREEN)
			{
				RunInitialization();
			}
			else
			{
				m_gameScreen->Initialize();
			}
		}
		break;
	case GAME_SCREEN:
		m_state = (ApplicationState)m_gameScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != GAME_SCREEN)
		{
			RunInitialization();
		}
		break;
	case JOIN_GAME_SCREEN:
		m_state = (ApplicationState)m_joinGameScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != JOIN_GAME_SCREEN)
		{
			RunInitialization();
		}
		break;
	case OPTIONS_SCREEN:
		m_state = (ApplicationState)m_optionsScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != OPTIONS_SCREEN)
		{
			RunInitialization();
		}
		break;
	case MAIN_MENU_SCREEN:
		m_state = (ApplicationState)m_mainMenuScreen->Update(m_DeltaTime,p_userCMDs);
		if (m_state != MAIN_MENU_SCREEN)
		{
			RunInitialization();
		}
		break;
	case SHUT_DOWN:
		exit(1337);
		break;
	default:
		break;
	}
}
//callback inte helt fixat då den inte får ligga som en medlemsfunktion, och måste därför vara static => vilket gör att den inte kan kalla på medlemsfunktioner, kan fixas med att lägga den i ett namespace och trixa med "this" , eller ha den i main där allt är static och kan skriva funktioner som inte behöver en klass
float t_bajs=0;
void OnMouseMove(WPARAM btnStae, int x, int y)
{
	if (btnStae & MK_LBUTTON != 0)
	{
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_LastMousePos.y));
		
		
		t_bajs+=m_DeltaTime;
		//m_GraphicHandle->UpdateCamera(m_ActiveCamera,0,0,0,dy,dx);
		//m_GraphicHandle->UpdateCameraVehicleSelection(m_ActiveCamera,t_bajs);
		//m_GraphicHandle->ChangeLevelSelection(0);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}

void OnKeyMove()
{
	if(GetAsyncKeyState('W')&0x8000)
	{
		m_GraphicHandle->UpdateCamera(m_ActiveCamera,100*m_DeltaTime,0,0,0,0);
	}
	if(GetAsyncKeyState('S')&0x8000)
	{
		m_GraphicHandle->UpdateCamera(m_ActiveCamera,-100*m_DeltaTime,0,0,0,0);
	}
	if(GetAsyncKeyState('A')&0x8000)
	{
		m_GraphicHandle->UpdateCamera(m_ActiveCamera,0,-100*m_DeltaTime,0,0,0);
	}
	if(GetAsyncKeyState('D')&0x8000)
	{
		m_GraphicHandle->UpdateCamera(m_ActiveCamera,0,100*m_DeltaTime,0,0,0);
	}
	if(GetAsyncKeyState('1')&0x8000)
	{
		m_ActiveCamera = 0;
	}
	if(GetAsyncKeyState('2')&0x8000)
	{
		m_ActiveCamera = 1;
	}
	if(GetAsyncKeyState('3')&0x8000)
	{
		m_ActiveCamera = 2;
	}
	if(GetAsyncKeyState('4')&0x8000)
	{
		m_ActiveCamera = 3;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
			m_GraphicHandle->Cleanup();

            PostQuitMessage( 0 );
            break;
		case WM_KEYDOWN:
			OnKeyMove();
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
			break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

HRESULT InitializeWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )107 );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW  ); //IDC_ARROW IDC_NO
	//ShowCursor(false);
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"Pipe Panic";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    handleInstance = hInstance;

	RECT t_rc = { 0, 0, 1920, 1080};
	AdjustWindowRect(&t_rc, WS_CAPTION, false);

	
    //AdjustWindowRect( &t_rc, WS_OVERLAPPEDWINDOW, FALSE );
    m_HandleWindow = CreateWindow(  wcex.lpszClassName,  wcex.lpszClassName, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, t_rc.right - t_rc.left, t_rc.bottom - t_rc.top, nullptr, nullptr, hInstance,
                           nullptr );
    if( !m_HandleWindow )
        return E_FAIL;
	
    ShowWindow( m_HandleWindow, nCmdShow );
	//ChangeDisplaySettingsA(NULL, CDS_FULLSCREEN);

    return S_OK;
}


void CleanUpCrew()
{
	//m_GraphicHandle->Cleanup();
}
