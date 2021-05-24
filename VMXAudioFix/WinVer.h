// Must set version macros before any includes to enable Windows 2000+ features

#pragma once

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE

#define WINVER          0x0500
#define _WIN32_WINNT    0x0500
#define _WIN32_IE       0x0500
