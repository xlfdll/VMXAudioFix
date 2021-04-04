#pragma once

#pragma comment(lib, "dxguid.lib")

#include <Windows.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "helper.h"

HRESULT InitializeDirectMusicWDM();
HRESULT ScanDirectMusicWDMPort();
HRESULT CreateDirectMusicWDMPort(DMUS_PORTCAPS* pDirectMusicPortCaps);
void ReleaseDirectMusicWDM();