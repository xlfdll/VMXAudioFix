#include "WinVer.h"

#include <Windows.h>

#include "tstring.h"

#include "Resource.h"

#include "WndProc.h"
#include "DXMusic.h"

#include "WinHelper.h"
#include "Diags.h"

int WINAPI WinMain
	(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	std::tstring appID = LoadResourceString(hInstance, IDS_APP_ID);
	std::tstring appTitle = LoadResourceString(hInstance, IDS_APP_TITLE);

    if (!CheckOSVersion())
    {
        std::tstring wrongOSVersionError
            = LoadResourceString(hInstance, IDS_WRONG_OS_VERSION_ERROR);

        MessageBox(NULL, wrongOSVersionError.c_str(),
            appTitle.c_str(), MB_ICONERROR);

        return 0;
    }
    if (!CheckSingleInstance())
    {
        std::tstring duplicateInstanceError
            = LoadResourceString(hInstance, IDS_DUPLICATE_INSTANCE_ERROR);

        MessageBox(NULL, duplicateInstanceError.c_str(),
            appTitle.c_str(), MB_ICONWARNING);

        return 0;
    }

    // Refresh Run On Startup registry value to match current executable path
    if (GetRunOnStartupState(hInstance))
    {
        SetRunOnStartupState(hInstance, FALSE);
        SetRunOnStartupState(hInstance, TRUE);
    }

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = appID.c_str();

	if (!RegisterClass(&wndclass))
	{
        ShowCustomErrorMessageBox(hInstance, NULL,
            TEXT("RegisterClass() failed"));

		return 0;
	}

	hwnd = CreateWindow(
		appID.c_str(),
		appTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd)
	{
        HRESULT hResult = InitializeDirectMusicWDM(hwnd);

        if (hResult != S_OK)
        {
            OutputDebugString
                (TEXT("[VMXAudioFix] InitializeDirectMusicWDM() failed.\n"));

            ShowCustomErrorMessageBox
                (hInstance, hwnd, TEXT("InitializeDirectMusicWDM() failed."));
        }
        else
        {
		    ShowWindow(hwnd, SW_HIDE);

		    while (GetMessage(&msg, NULL, 0, 0))
		    {
			    TranslateMessage(&msg);
			    DispatchMessage(&msg);
		    }
        
            ReleaseDirectMusicWDM();
        }
	}

	return msg.wParam;
}