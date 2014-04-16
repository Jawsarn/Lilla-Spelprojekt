
#include <Windows.h>
#include "Controller.h"
#include <vector>
#include "UserCMD.h"
#include <DirectXMath.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT InitializeWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow, UINT width, UINT height);
void Run();

HINSTANCE	handleInstance;
HWND	handleWindow;


std::vector<UserCMD> userCMDS;

float deltaTime;
float gameTime;
ULONGLONG prevTime;

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	InitializeWindow(hInstance,nCmdShow,1024,1024);

	Run();

	return 0;
}


void Run() 
{
	Controller p1_controller= Controller(0);
	
	UserCMD t_p1UserCMD;
	userCMDS.push_back(t_p1UserCMD);
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
			if (p1_controller.IsConnected())
			{
				XINPUT_STATE p1_state = p1_controller.GetState();
				DirectX::XMFLOAT2 p1_leftJoystick = DirectX::XMFLOAT2(p1_controller.CheckMovmentStickLeft().x,p1_controller.CheckMovmentStickLeft().y);

				if (p1_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					userCMDS[0].aButtonPressed = true;
				}
				else
				{
					userCMDS[0].aButtonPressed = false;
				}
				if (p1_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				{
					userCMDS[0].bButtonPressed = true;
				}
				else
				{
					userCMDS[0].bButtonPressed = false;
				}
				if (p1_state.Gamepad.bLeftTrigger & VK_PAD_LTRIGGER)
				{
					p1_controller.Vibrate(64000,0);
				}
				if (p1_state.Gamepad.bRightTrigger & VK_PAD_RTRIGGER)
				{
					p1_controller.Vibrate(0,0);
				}
				if (p1_leftJoystick.x != 0, p1_leftJoystick.y !=0)
				{
					userCMDS[0].Joystick = p1_leftJoystick;
				}

			}
			
			ULONGLONG timeCur = GetTickCount64();
			if( prevTime == 0 )
				prevTime = timeCur;
			deltaTime = ( timeCur - prevTime ) / 1000.0f;
			gameTime += deltaTime;
			prevTime = timeCur;

			///UPDATE & DRAW
		}
	}
	if (p1_controller.IsConnected())
	{
		p1_controller.Vibrate(0,0);
	}
}

//callback inte helt fixat då den inte får ligga som en medlemsfunktion, och måste därför vara static => vilket gör att den inte kan kalla på medlemsfunktioner, kan fixas med att lägga den i ett namespace och trixa med "this" , eller ha den i main där allt är static och kan skriva funktioner som inte behöver en klass

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
            PostQuitMessage( 0 );
            break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_MOUSEMOVE:
			break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

HRESULT InitializeWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow, UINT width, UINT height)
{
	WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )107 );
    wcex.hCursor = LoadCursor( nullptr, IDC_NO  );
	ShowCursor(false);
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"Pipe Panic";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    handleInstance = hInstance;
    RECT rc = { 0, 0, width, height };
	
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    handleWindow = CreateWindow(  wcex.lpszClassName,  wcex.lpszClassName, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );
    if( !handleWindow )
        return E_FAIL;
	
    ShowWindow( handleWindow, nCmdShow );
	//ChangeDisplaySettingsA(NULL, CDS_FULLSCREEN);

    return S_OK;
}


