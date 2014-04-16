
#include <Windows.h>
#include "Controller.h"
#include <vector>
#include "UserCMD.h"
#include <DirectXMath.h>
#include "UserCMDHandler.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT InitializeWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow, UINT width, UINT height);
void Run();

HINSTANCE	handleInstance;
HWND	handleWindow;




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
	std::vector<UserCMD> userCMDS;
	UserCMDHandler userCMDHandler = UserCMDHandler();

	for (int i = 0; i < 4; i++)
	{
		UserCMD t_userCMD = UserCMD(i);
		userCMDS.push_back(t_userCMD);
	}
	
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
			for (int i = 0; i < 4; i++) ///Fixes UserCMDs for all connected players
			{
				if (userCMDS[i].controller.IsConnected())
				{
					userCMDHandler.AlterUserCMD(userCMDS[i]);
				} 
				else
				{
					////Player i controller disconnected, plz connect again message
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


