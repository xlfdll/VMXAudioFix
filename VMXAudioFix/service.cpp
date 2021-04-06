#include "service.h"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR* argv)
{
	DWORD status;
	DWORD specificError;

	ZeroMemory(&ServiceStatus, sizeof(SERVICE_STATUS));

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	ServiceStatusHandle = RegisterServiceCtrlHandler
		(VMXAUDIOFIX_SERVICE_NAME,
		ServiceCtrlHandler);

	if (ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] RegisterServiceCtrlHandler() failed (%lu).\n"),
			GetLastError());

		return;
	}

	status = ServiceInitialization(argc, argv, &specificError);

	if (status != NO_ERROR)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] Service initialization failed (%lu).\n"),
			status);

		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = status;
		ServiceStatus.dwServiceSpecificExitCode = specificError;

		SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

		return;
	}

	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
	{
		status = GetLastError();

		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] SetServiceStatus() failed (%lu).\n"),
			status);
	}

	OutputDebugString
        (TEXT("[VMXAudioFixService] Service is initialized and running.\n"));

	return;
}

VOID WINAPI ServiceCtrlHandler (DWORD opcode)
{
	DWORD status;

	switch (opcode)
	{
	case SERVICE_CONTROL_STOP:
		ServiceCleanup();
		
		OutputDebugString
			(TEXT("[VMXAudioFixService] Stopping service...\n"));

		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;

		if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
		{
			status = GetLastError();
			
			ServiceDebugOutDWORD
				(TEXT("[VMXAudioFixService] SetServiceStatus() failed (%lu).\n"),
				status);
		}

		OutputDebugString
            (TEXT("[VMXAudioFixService] Service exited.\n"));

		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		ServiceDebugOutDWORD
            (TEXT("[VMXAudioFixService] Unrecognized opcode %lu.\n"), opcode);
		break;
	}

	// Send current status
	if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
	{
		status = GetLastError();

		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] SetServiceStatus() failed (%lu).\n"),
			status);
	}

	return;
}

DWORD ServiceInitialization (DWORD argc, LPTSTR* argv, DWORD* specificError)
{
	HRESULT hResult = InitializeDirectMusicWDM();

	if (hResult != S_OK)
	{
		OutputDebugString
			(TEXT("[VMXAudioFixService] InitializeDirectMusicWDM() failed.\n"));

        *specificError = hResult;

		return ERROR_GEN_FAILURE;
	}

    *specificError = 0;

	return NO_ERROR;
}

VOID ServiceCleanup ()
{
	ReleaseDirectMusicWDM();
}