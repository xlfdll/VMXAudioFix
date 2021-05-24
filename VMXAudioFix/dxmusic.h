#pragma once

#pragma comment(lib, "dxguid.lib")

#include <Windows.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "Globals.h"
#include "Diags.h"

HRESULT InitializeDirectMusicWDM(HWND hwnd);
HRESULT ScanDirectMusicWDMPort();
HRESULT CreateDirectMusicWDMPort(DMUS_PORTCAPS* pDirectMusicPortCaps);
void ReleaseDirectMusicWDM();