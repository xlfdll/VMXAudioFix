#include "setup.h"

BOOL InstallService()
{
	TCHAR sourcePath[MAX_PATH];

	GetModuleFileName(NULL, sourcePath, MAX_PATH);

    puts("Service path:");
    _tprintf(TEXT("%s\n"), sourcePath);
    puts("");

	SC_HANDLE scManagerHandle = OpenSCManager
		(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (scManagerHandle == NULL)
	{
		puts("ERROR: OpenSCManager() failed.");

		return FALSE;
	}

	SC_HANDLE scServiceHandle = CreateService
		(scManagerHandle,
		VMXAUDIOFIX_SERVICE_NAME,
		VMXAUDIOFIX_SERVICE_DISPLAY_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		sourcePath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (scServiceHandle == NULL)
	{
        DWORD lastError = GetLastError();

        if (lastError == ERROR_SERVICE_EXISTS)
        {
            puts("ERROR: Service is already installed on the current system. Uninstall it first to reinstall.");
        }
        else
        {
		    puts("ERROR: CreateService() failed");
        }

		CloseServiceHandle(scManagerHandle);

		return FALSE;
	}

	CloseServiceHandle(scServiceHandle);
	CloseServiceHandle(scManagerHandle);

	puts("Service has been installed successfully.");

	return TRUE;
}

BOOL UninstallService()
{
	SC_HANDLE scManagerHandle = OpenSCManager
		(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	
	if (scManagerHandle == NULL)
	{
		puts("ERROR: OpenSCManager() failed.");

		return FALSE;
	}

	SC_HANDLE scServiceHandle = OpenService
		(scManagerHandle, VMXAUDIOFIX_SERVICE_NAME, DELETE);

	if (scServiceHandle == NULL)
	{
		DWORD lastError = GetLastError();

        if (lastError == ERROR_SERVICE_DOES_NOT_EXIST)
        {
            puts("ERROR: Service is not installed on the current system.");
        }
		else if (lastError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			puts("ERROR: Service is marked for deletion on the current system. Please restart the computer.");
		}
        else
        {
		    puts("ERROR: OpenService() failed");
        }

		CloseServiceHandle(scManagerHandle);

		return FALSE;
	}

	if (!DeleteService(scServiceHandle))
	{
		puts("ERROR: DeleteService() failed.");

		CloseServiceHandle(scServiceHandle);
		CloseServiceHandle(scManagerHandle);

		return FALSE;
	}

	CloseServiceHandle(scServiceHandle);
	CloseServiceHandle(scManagerHandle);

	puts("Service has been uninstalled from the current system.");

	return TRUE;
}