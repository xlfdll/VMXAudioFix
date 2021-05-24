#include "WndProc.h"

LRESULT CALLBACK WndProc
	(HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

	switch (message)
	{
    case WM_CREATE:
        if (!CreateTrayIcon(hInstance, hwnd))
        {
            ShowCustomErrorMessageBox(hInstance, hwnd, TEXT("CreateTrayIcon() failed"));

            return -1;
        }

        return 0;
    case WM_APP_NOTIFYICON_CALLBACK:
        switch (LOWORD(lParam))
        {
        case WM_CONTEXTMENU:
            POINT point;

            // For some reason the Windows classic sample uses wParam (always 0)
            // Use GetCursorPos() function here to get correct mouse cursor location
            if (GetCursorPos(&point))
            {
                ShowTrayIconMenu(hInstance, hwnd, point);
            }

            break;
        }

        return 0;
    case WM_COMMAND:
        HandleMenuItem(hInstance, hwnd, message, wParam, lParam);

        return 0;
	case WM_DESTROY:
        DestroyTrayIcon(hwnd);
		PostQuitMessage(0);

		return 0;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void HandleMenuItem
    (HINSTANCE hInstance,
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    int command = LOWORD(wParam);

    switch (command)
    {
    case IDR_MENU_ITEM_RUN_ON_STARTUP:
        if (SetRunOnStartupState
            (hInstance, !GetRunOnStartupState(hInstance))
            != ERROR_SUCCESS)
        {
            ShowSystemLastErrorMessageBox(hInstance, hwnd);
        }

        break;
    case IDR_MENU_ITEM_ABOUT:
        DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)AboutProc);

        break;
    case IDR_MENU_ITEM_EXIT:
        PostMessage(hwnd, WM_CLOSE, NULL, NULL);
        
        break;
    }
}

BOOL CALLBACK AboutProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        TCHAR srcBuffer[1024];
        TCHAR dstBuffer[1024];

        GetDlgItemText(hwndDlg, IDC_APP_VERSION, srcBuffer, sizeof(srcBuffer));
        _stprintf(dstBuffer, srcBuffer, GetProductVersion().c_str());
        SetDlgItemText(hwndDlg, IDC_APP_VERSION, dstBuffer);

        GetDlgItemText(hwndDlg, IDC_PORT_STATUS, srcBuffer, sizeof(srcBuffer));
        _stprintf(dstBuffer, srcBuffer, SelectedDXMusicPort.c_str());
        SetDlgItemText(hwndDlg, IDC_PORT_STATUS, dstBuffer);

        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwndDlg, wParam);

            return TRUE;
        }
    }

    return FALSE;
}