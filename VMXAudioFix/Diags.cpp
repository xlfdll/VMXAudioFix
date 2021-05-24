#include "Diags.h"

void ShowCustomErrorMessageBox(HINSTANCE hInstance, HWND hwnd, PCTSTR szError)
{
    std::tstring appTitle = LoadResourceString(hInstance, IDS_APP_TITLE);

    TCHAR messageBuffer[2048];
    std::tstring errorMessageFormat = LoadResourceString(hInstance, IDS_UNEXPECTED_ERROR);
    _stprintf(messageBuffer, errorMessageFormat.c_str(), szError);

    MessageBox(hwnd, messageBuffer, appTitle.c_str(), MB_ICONERROR);
}

void ShowSystemLastErrorMessageBox(HINSTANCE hInstance, HWND hwnd)
{
    std::tstring appTitle = LoadResourceString(hInstance, IDS_APP_TITLE);
    LPVOID messageBuffer;

    FormatMessage
        (FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&messageBuffer,
        0,
        NULL);

    MessageBox(hwnd, (LPTSTR)messageBuffer, appTitle.c_str(), MB_ICONERROR);

    LocalFree(messageBuffer);
}

void ServiceDebugOutHRESULT(LPCTSTR format, HRESULT hResult)
{
	TCHAR buffer[2048];

	_stprintf(buffer, format, hResult);

	OutputDebugString(buffer);
}

void ServiceDebugOutTSTR(LPCTSTR format, LPCTSTR str)
{
	TCHAR buffer[2048];

	_stprintf(buffer, format, str);

    OutputDebugString(buffer);
}