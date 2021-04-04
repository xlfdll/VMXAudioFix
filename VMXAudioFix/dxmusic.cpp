#include "dxmusic.h"

IDirectMusic* pDirectMusic;
IDirectMusicPerformance* pDirectMusicPerformance;
IDirectMusicPort* pDirectMusicPort;

HRESULT InitializeDirectMusicWDM()
{
	if (FAILED(CoInitialize(NULL)))
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] CoInitialize(NULL) error.\n"), 0);

		return E_FAIL;
	}

	if (FAILED(CoCreateInstance
		(CLSID_DirectMusicPerformance,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicPerformance2,
		(void**)&pDirectMusicPerformance)))
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] DirectMusic performance object creation failed.\n"), 0);

		pDirectMusicPerformance = NULL;

		return E_FAIL;
	}

	pDirectMusic = NULL;

	if (FAILED(pDirectMusicPerformance->Init
		(&pDirectMusic,
		NULL,
		GetDesktopWindow())))
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] DirectMusic object creation failed.\n"), 0);

		pDirectMusic = NULL;

		return E_FAIL;
	}

	if (ScanDirectMusicWDMPort() != S_OK)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] DirectMusic port setup failed.\n"), 0);

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
					(TEXT("[VMXAudioFixService] DirectMusic port %s is created and activated.\n"), portCaps.wszDescription);	
			}
			else
			{
				ServiceDebugOutTSTR
					(TEXT("[VMXAudioFixService] DirectMusic port %s creation failed.\n"), portCaps.wszDescription);
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
				(TEXT("[VMXAudioFixService] DirectMusic port enumeration failed (%ld).\n"), hResult);

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
			(TEXT("[VMXAudioFixService] DirectMusic CreatePort() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusic->Activate(TRUE);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFixService] DirectMusic Activate() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusicPerformance->AddPort(pDirectMusicPort);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFixService] DirectMusic AddPort() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	hResult = pDirectMusicPerformance->AssignPChannelBlock(0, pDirectMusicPort, 1);

	if (FAILED(hResult))
	{
		ServiceDebugOutHRESULT
			(TEXT("[VMXAudioFixService] DirectMusic AssignPChannelBlock() failed (%ld).\n"), hResult);
		
		return hResult;
	}

	return S_OK;
}

void ReleaseDirectMusicWDM()
{
    ServiceDebugOutDWORD
        (TEXT("[VMXAudioFixService] Cleaning up DirectMusic components...\n"), 0);

	if (pDirectMusicPerformance)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] Cleaning up DirectMusic performance object...\n"), 0);

		pDirectMusicPerformance->CloseDown();
		pDirectMusicPerformance->Release();
		pDirectMusicPerformance = NULL;
	}

	if (pDirectMusicPort)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] Cleaning up DirectMusic port...\n"), 0);

		pDirectMusicPort->Release();
		pDirectMusicPort = NULL;
	}

	if (pDirectMusic)
	{
		ServiceDebugOutDWORD
			(TEXT("[VMXAudioFixService] Cleaning up DirectMusic object...\n"), 0);

		pDirectMusic->Release();
		pDirectMusic = NULL;
	}

	CoUninitialize();
}