#include "helper.h"

VOID ServiceDebugOutDWORD(LPCTSTR format, DWORD status)
{
	TCHAR buffer[2048];

	_stprintf(buffer, format, status);

	OutputDebugString(buffer);
}

VOID ServiceDebugOutHRESULT(LPCTSTR format, HRESULT hResult)
{
	TCHAR buffer[2048];

	_stprintf(buffer, format, hResult);

	OutputDebugString(buffer);
}

VOID ServiceDebugOutTSTR(LPCTSTR format, LPCTSTR str)
{
	TCHAR buffer[2048];

	_stprintf(buffer, format, str);

    OutputDebugString(buffer);
}
