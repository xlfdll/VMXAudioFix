#include "WinHelper.h"

BOOL GetOSVersionInfo(OSVERSIONINFOEX* pOSVersionInfo)
{
    if (pOSVersionInfo == NULL)
    {
        return FALSE;
    }

    ZeroMemory(pOSVersionInfo, sizeof(OSVERSIONINFOEX));

    pOSVersionInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    return GetVersionEx((OSVERSIONINFO*)pOSVersionInfo);
}

BOOL CheckOSVersion()
{
    OSVERSIONINFOEX osVersionInfo;

    if (GetOSVersionInfo(&osVersionInfo))
    {
        return (osVersionInfo.dwMajorVersion == 5);
    }
    else
    {
        return FALSE;
    }
}

BOOL CheckSingleInstance()
{
    HANDLE mutex = CreateMutex(NULL, FALSE,
        TEXT("Global\\VMXAUDIOFIX_SINGLE_INSTANCE_MUTEX"));

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(mutex);

        return FALSE;
    }

    return TRUE;
}

std::tstring LoadResourceString(HINSTANCE hInstance, UINT uID)
{
	PTSTR pStr;

	int rVal = LoadString(hInstance, uID, (PTSTR)&pStr, 0);

	if (rVal)
	{
		return std::tstring(pStr, rVal);
	}
	else
	{
		return std::tstring();
	}
}

BOOL CreateTrayIcon(HINSTANCE hInstance, HWND hwnd)
{
    NOTIFYICONDATA data;
    OSVERSIONINFOEX osVersionInfo;
    BOOL isWhistler = (GetOSVersionInfo(&osVersionInfo)
        && osVersionInfo.dwMinorVersion == 1);

    data.cbSize = sizeof(NOTIFYICONDATA);
    data.hWnd = hwnd;
    data.uID = 0;
    data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    data.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE
        (isWhistler ? IDI_APP_ICON : IDI_APP_ICON_SMALL_2K));
    data.uCallbackMessage = WM_APP_NOTIFYICON_CALLBACK;

    LoadString(hInstance, IDS_APP_TITLE, data.szTip, sizeof(data.szTip));

    Shell_NotifyIcon(NIM_ADD, &data);

    // Need to set version explicitly to support new features (such as context menu)
    data.uVersion = NOTIFYICON_VERSION;
    return Shell_NotifyIcon(NIM_SETVERSION, &data);
}

BOOL DestroyTrayIcon(HWND hwnd)
{
    NOTIFYICONDATA data;

    data.cbSize = sizeof(NOTIFYICONDATA);
    data.hWnd = hwnd;
    data.uID = 0;

    return Shell_NotifyIcon(NIM_DELETE, &data);
}

void ShowTrayIconMenu(HINSTANCE hInstance, HWND hwnd, POINT point)
{
    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU_TRAY_ICON));

    if (hMenu)
    {
        HMENU hSubMenu = GetSubMenu(hMenu, 0);

        if (hSubMenu)
        {        
            SetForegroundWindow(hwnd);

            UINT uFlags = TPM_RIGHTBUTTON;

            if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
            {
                uFlags |= TPM_RIGHTALIGN;
            }
            else
            {
                uFlags |= TPM_LEFTALIGN;
            }

            SetRunOnStartupMenuItemState(hSubMenu, GetRunOnStartupState(hInstance));

            // Popup menu must be a submenu under an existing menu
            // Otherwise no menu items would be shown
            TrackPopupMenuEx(hSubMenu, uFlags, point.x, point.y, hwnd, NULL);
        }

        DestroyMenu(hMenu);
    }
}

BOOL SetRunOnStartupMenuItemState(HMENU hMenu, BOOL state)
{
    if (hMenu)
    {
        MENUITEMINFO menuItemInfo;
        menuItemInfo.cbSize = sizeof(MENUITEMINFO);
        menuItemInfo.fMask = MIIM_STATE;

        menuItemInfo.fState = state ? MFS_CHECKED : MFS_UNCHECKED;

        if (!SetMenuItemInfo
            (hMenu,
            IDR_MENU_ITEM_RUN_ON_STARTUP,
            FALSE,
            &menuItemInfo))
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL GetRunOnStartupState(HINSTANCE hInstance)
{
    std::tstring appTitle = LoadResourceString(hInstance, IDS_APP_TITLE);
    HKEY hKey;
    BOOL result;

    LONG status = RegOpenKeyEx
        (HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
        0,
        KEY_READ,
        &hKey);

    if (status != ERROR_SUCCESS)
    {
        return FALSE;
    }

    status = RegQueryValueEx
        (hKey, appTitle.c_str(), NULL, NULL, NULL, NULL);

    result = (status == ERROR_SUCCESS);

    RegCloseKey(hKey);

    return result;
}

LONG SetRunOnStartupState(HINSTANCE hInstance, BOOL runOnStartup)
{
    std::tstring appTitle = LoadResourceString(hInstance, IDS_APP_TITLE);
    TCHAR currentProcessPath[MAX_PATH + 1];

    GetModuleFileName(NULL, currentProcessPath, MAX_PATH);

    HKEY hKey;

    LONG status = RegOpenKeyEx
        (HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
        0,
        KEY_WRITE,
        &hKey);

    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (runOnStartup)
    {
        status = RegSetValueEx
            (hKey,
            appTitle.c_str(),
            0,
            REG_SZ,
            (LPCBYTE)currentProcessPath,
            (lstrlen(currentProcessPath) + 1) * sizeof(TCHAR));
    }
    else
    {
        status = RegDeleteValue(hKey, appTitle.c_str());
    }

    return status;
}

std::tstring GetProductVersion()
{
    std::tstringstream ss;

    TCHAR currentProcessPath[MAX_PATH + 1];

    GetModuleFileName(NULL, currentProcessPath, MAX_PATH);
    
    DWORD dummy;
    DWORD verInfoSize = GetFileVersionInfoSize(currentProcessPath, &dummy);

    if (verInfoSize > 0)
    {
        BYTE* verInfoBuffer = (BYTE*)LocalAlloc(LPTR, verInfoSize);

        if (GetFileVersionInfo(currentProcessPath, NULL, verInfoSize, verInfoBuffer))
        {
            LPVOID productVersion;
            UINT productVersionLength;

            if (VerQueryValue
                (verInfoBuffer,
                TEXT("\\StringFileInfo\\040904b0\\ProductVersion"),
                &productVersion,
                &productVersionLength))
            {
                ss << (LPCTSTR)productVersion;

                LocalFree(verInfoBuffer);
            }
        }
    }

    std::tstring result = ss.str();

    return result.size() > 0 ? result : std::tstring(TEXT("N/A"));
}