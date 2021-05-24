#pragma once

#pragma comment(lib, "version.lib")

#include "WinVer.h"

#include <Windows.h>
#include <tchar.h>

#include "tstring.h"

#include "Resource.h"

#include "Constants.h"

BOOL GetOSVersionInfo(OSVERSIONINFOEX* pOSVersionInfo);
BOOL CheckOSVersion();

BOOL CheckSingleInstance();

std::tstring LoadResourceString(HINSTANCE hInstance, UINT uID);

BOOL CreateTrayIcon(HINSTANCE hInstance, HWND hwnd);
BOOL DestroyTrayIcon(HWND hwnd);

void ShowTrayIconMenu(HINSTANCE hInstance, HWND hwnd, POINT point);
BOOL SetRunOnStartupMenuItemState(HMENU hMenu, BOOL state);

BOOL GetRunOnStartupState(HINSTANCE hInstance);
LONG SetRunOnStartupState(HINSTANCE hInstance, BOOL runOnStartup);

std::tstring GetProductVersion();