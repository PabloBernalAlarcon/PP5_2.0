// PP5_Base_App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PP5_Base_App.h"
#include <iostream>
#include "DirectXVault.h"
#include <chrono>
#include "FBXInteraction.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;    
FBXinteracts::Functions yee;
std::vector<float> Bones;
std::vector<float> vertices;
std::vector<FBXinteracts::vert> verticestemp;
// current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;
int width = 1080;
int height = 920;
DirectXVault DXVault;
std::vector<DirectXVault::vertex> vertFinal;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PP5_BASE_APP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//Immediately after the call to InitInstance(but outside the scope of the if - check), add the following code :
	//This will spawn a console at start-up with our window when in debug mode.
#ifndef NDEBUG
	AllocConsole();
	FILE* new_std_in_out;
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
	std::cout << "Hello world!\n";
	yee.SetupFBX();
	
	std::vector<uint32_t> jimmy = yee.getIndices();
	verticestemp = yee.getBecky();

	for (int i = 0; i < verticestemp.size(); i++)
	{ 
		DirectXVault::vertex temp;
		temp.Position.x = verticestemp[i].Position[0];
		temp.Position.y = verticestemp[i].Position[1];
		temp.Position.z = verticestemp[i].Position[2];
		temp.Position.w = 1;

		temp.Color.x = verticestemp[i].Color[0];
		temp.Color.y = verticestemp[i].Color[1];
		temp.Color.z = verticestemp[i].Color[2];
		temp.Color.w = verticestemp[i].Color[3];

		temp.Normals.x = verticestemp[i].Normals[0];
		temp.Normals.y = verticestemp[i].Normals[1];
		temp.Normals.z = verticestemp[i].Normals[2];
		temp.Normals.w = verticestemp[i].Normals[3];

		vertFinal.push_back(temp);
	}
	for (int i = 0; i < yee.getvertsSize(); i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			vertices.push_back(yee.getverts(i, j));
		}
	}
	DXVault.Start(hWnd,vertFinal,jimmy);
	//std::cout << yee.GimmeSomething();
	
#endif

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PP5_BASE_APP));

    MSG msg;
	int key = 0;
	//std::vector<int> jimmy = yee.getIndices();
	
	bool stopYouCuck = false;
	std::chrono::time_point<std::chrono::system_clock> notNow;
	notNow = std::chrono::system_clock::now();
	float totalTime = 0;
    // Main message loop:
	FBXinteracts::AnimClip anims = yee.getAnimation();
    while (!stopYouCuck)
    {
		totalTime += (std::chrono::system_clock::now() - notNow).count() / 10000000.0f;
		notNow = std::chrono::system_clock::now();

		while (PeekMessageA(&msg, nullptr, 0, 0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				stopYouCuck = true;
			}
        }

		if (GetAsyncKeyState('T') & 0x01)
		{
			DXVault.toggle = !DXVault.toggle;
		}
		/*jimmy.clear();
		jimmy = yee.getAnimation().keys[key].bones;*/
		
		float lastTime = anims.keys[anims.keys.size() - 1].time;
		float mtime = fmod(totalTime, lastTime);
		int frame = 0;
		for (int i = 0; i < anims.keys.size(); i++)
		{
			if (anims.keys[i].time > mtime)
			{
				frame = i;
				break;
			}
		}
		int prevFrame = frame - 1;
		if (prevFrame < 0)
		{
			prevFrame = anims.keys.size() - 1;
		}
		float timeDiff = anims.keys[frame].time - anims.keys[prevFrame].time;;
		float timeIn= mtime - anims.keys[prevFrame].time;
		if (frame == 0)
		{
			 /*timeDiff = anims.keys[frame].time - anims.keys[prevFrame].time;
			 timeIn = mtime - anims.keys[prevFrame].time;*/
			timeDiff = anims.keys[frame].time;
			timeIn =   mtime;
		}

		float ratio = timeIn / timeDiff;
		std::vector<FBXinteracts::vert> CurrJoints; // yee.getAnimation().keys[prevFrame].bones;
		for (int i = 0; i < anims.keys[prevFrame].bones.size(); i++)
		{
			FBXinteracts::vert v1, v2,v3;
			v1 = anims.keys[prevFrame].bones[i];
			v2 = anims.keys[frame].bones[i];
			v3.Position[0] = (v2.Position[0] - v1.Position[0])*ratio + v1.Position[0];
			v3.Position[1] = (v2.Position[1] - v1.Position[1])*ratio + v1.Position[1];
			v3.Position[2] = (v2.Position[2] - v1.Position[2])*ratio + v1.Position[2];
			v3.Position[3] = 1.0f;

			CurrJoints.push_back(v3);
		}
		//FBXinteracts::AnimClip somso = yee.getAnimation();
		//yee.getAnim();
		//MeshPositions = yee.getPositions();
		Bones.clear();
		for (int i = 0; i < /*jimmy.size()*/CurrJoints.size(); i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Bones.push_back(/*jimmy*/CurrJoints[i].Position[j]);
			}

		}
		DXVault.bufferdemBones(Bones);
		DXVault.Render();
		CurrJoints.clear();
		if (GetAsyncKeyState('E') & 0x01)
		{
			key++;
		}
		if (key >= CurrJoints.size())
		{
			key = 0;
		}
    }
	//Add the following code immediately before the final return in the wWinMain function:
#ifndef NDEBUG
	FreeConsole();
#endif
	//-----------------------------

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PP5_BASE_APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PP5_BASE_APP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0,WIDTH,HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
void SetNewClick() {
	GetCursorPos(&DXVault.getCurrMousePos());
	ScreenToClient(hWnd, &DXVault.getCurrMousePos());
	DXVault.setCurrMouse(DXVault.getCurrMousePos());
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    //case WM_COMMAND:
    //    {
    //        int wmId = LOWORD(wParam);
    //        // Parse the menu selections:
    //        switch (wmId)
    //        {
    //        case IDM_ABOUT:
    //            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
    //            break;
    //        case IDM_EXIT:
    //            DestroyWindow(hWnd);
    //            break;
    //        default:
    //            return DefWindowProc(hWnd, message, wParam, lParam);
    //        }
    //    }
    //    break;
    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: Add any drawing code that uses hdc here...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
	/*case WM_RBUTTONDOWN:
		DXVault.KeyPressed(true);
		SetNewClick();
		break;
	case WM_RBUTTONUP:
		DXVault.KeyPressed(false);
		break;
		*/
		
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
