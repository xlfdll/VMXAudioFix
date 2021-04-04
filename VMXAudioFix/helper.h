#pragma once

#include <Windows.h>
#include <tchar.h>

VOID ServiceDebugOutDWORD(LPCTSTR format, DWORD status);
VOID ServiceDebugOutHRESULT(LPCTSTR format, HRESULT hResult);
VOID ServiceDebugOutTSTR(LPCTSTR format, LPCTSTR str);