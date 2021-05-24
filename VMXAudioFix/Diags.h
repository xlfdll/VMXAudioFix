#pragma once

#include <Windows.h>
#include <tchar.h>

#include "tstring.h"

#include "Resource.h"

#include "WinHelper.h"

void ShowCustomErrorMessageBox(HINSTANCE hInstance, HWND hwnd, PCTSTR szError);
void ShowSystemLastErrorMessageBox(HINSTANCE hInstance, HWND hwnd);

void ServiceDebugOutHRESULT(LPCTSTR format, HRESULT hResult);
void ServiceDebugOutTSTR(LPCTSTR format, LPCTSTR str);