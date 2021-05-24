#include "DXMusic.h"

static IDirectMusic* pDirectMusic;
static IDirectMusicPerformance* pDirectMusicPerformance;
static IDirectMusicPort* pDirectMusicPort;

HRESULT InitializeDirectMusicWDM(HWND hwnd)
{
	if (FAILED(CoInitialize(NULL)))
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] CoInitialize(NULL) error.\n"));

		return E_FAIL;
	}

	if (FAILED(CoCreateInstance
		(CLSID_DirectMusicPerformance,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicPerformance2,
		(void**)&pDirectMusicPerformance)))
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] DirectMusic performance object creation failed.\n"));

		pDirectMusicPerformance = NULL;

		return E_FAIL;
	}

	pDirectMusic = NULL;

	if (FAILED(pDirectMusicPerformance->Init
		(&pDirectMusic,
		NULL,
		hwnd)))
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] DirectMusic object creation failed.\n"));

		pDirectMusic = NULL;

		return E_FAIL;
	}

	if (ScanDirectMusicWDMPort() != S_OK)
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] DirectMusic port setup failed.\n"));

		return E_FAIL;
	}

	return S_OK;
}

HRESULT ScanDirectMusicWDMPort()
{
	DMUS_PORTCAPS portCaps;
	HRESULT hResult = S_OK;
	UINT devIndex = 0;

	while (hResult == S_OK)
	{
		portCaps.dwSize = sizeof(DMUS_PORTCAPS);

		hResult = pDirectMusic->EnumPort(devIndex, &portCaps);

		if (hResult == S_OK
			&& portCaps.dwClass == DMUS_PC_OUTPUTCLASS
			&& portCaps.dwType == DMUS_PORT_KERNEL_MODE)
		{
			if (SUCCEEDED(CreateDirectMusicWDMPort(&portCaps)))
			{
				ServiceDebugOutTSTR
					(TEXT("[VMXAudioFix] DirectMusic port %s is created and activated.\n"), portCaps.wszDescription);

                SelectedDXMusicPort = std::tstring(portCaps.wszDescription);
			}
			else
			{
				ServiceDebugOutTSTR
					(TEXT("[VMXAudioFix] DirectMusic port %s creation failed.\n"), portCaps.wszDescription);
			}

			break;
		}
		else if (hResult == S_FALSE
			|| portCaps.dwClass == DMUS_PC_INPUTCLASS
			|| portCaps.dwType != DMUS_PORT_KERNEL_MODE)
		{
            ZeroMemory(&portCaps, sizeof(DMUS_PORTCAPS));
		}
		else
		{
			ServiceDebugOutHRESULT
				(TEXT("[VMXAudioFix] DirectMusic port enumeration failed (%ld).\n"), hResult);

			return hResult;
		}

		devIndex++;
	}

	return S_OK;
}

HRESULT CreateDirectMusicWDMPort(DMUS_PORTCAPS* pDirectMusicPortCaps)
{
	HRESULT hResult;
	DMUS_PORTPARAMS portParams;

	ZeroMemory(&portParams, sizeof(DMUS_PORTPARAMS));

    portParams.dwSize = sizeof(DMUS_PORTPARAMS);
    portParams.dwValidParams =
		DMUS_PORTPARAMS_CHANNELGROUPS | 
        DMUS_PORTPARAMS_EFFECTS;
	portParams.dwEffectFlags = 0;
	portParams.dwChannelGroups = 1;

	hResult = pDirectMusic->CreatePort
		(pDirectMusicPortCaps->guidPort,
		&portParams,
		&pDirectMusicPort,
		NULL);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFix] DirectMusic CreatePort() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusic->Activate(TRUE);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFix] DirectMusic Activate() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusicPerformance->AddPort(pDirectMusicPort);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFix] DirectMusic AddPort() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusicPerformance->AssignPChannelBlock(0, pDirectMusicPort, 1);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFix] DirectMusic AssignPChannelBlock() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	return S_OK;
}

void ReleaseDirectMusicWDM()
{
    OutputDebugString
        (TEXT("[VMXAudioFix] Cleaning up DirectMusic components...\n"));

	if (pDirectMusicPerformance)
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] Cleaning up DirectMusic performance object...\n"));

		pDirectMusicPerformance->CloseDown();
		pDirectMusicPerformance->Release();
		pDirectMusicPerformance = NULL;
	}

	if (pDirectMusicPort)
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] Cleaning up DirectMusic port...\n"));

		pDirectMusicPort->Release();
		pDirectMusicPort = NULL;
	}

	if (pDirectMusic)
	{
		OutputDebugString
			(TEXT("[VMXAudioFix] Cleaning up DirectMusic object...\n"));

		pDirectMusic->Release();
		pDirectMusic = NULL;
	}

	CoUninitialize();
}