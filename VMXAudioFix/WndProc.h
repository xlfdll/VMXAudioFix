#pragma once

#include <Windows.h>
#include <tchar.h>

#include "Resource.h"

#include "Globals.h"
#include "WinHelper.h"
#include "Diags.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void HandleMenuItem(HINSTANCE hInstance, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);