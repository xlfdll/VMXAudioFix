#pragma once

#include <Windows.h>

#include "dxmusic.h"
#include "constant.h"
#include "helper.h"

VOID WINAPI ServiceMain (DWORD argc, LPTSTR* argv);
VOID WINAPI ServiceCtrlHandler (DWORD opcode);
DWORD ServiceInitialization (DWORD argc, LPTSTR* argv, DWORD* specificError);
VOID ServiceCleanup ();