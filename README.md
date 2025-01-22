# VMware Audio Fix Service
A Windows service that attempts to fix audio distortion issue in Windows 2000/XP guests on VMware hypervisor

## System Requirements
* VMware hypervisor
* Windows 2000 or Windows XP guests

Already tested on following platforms:
* VMware Workstation 15/16
* Windows 2000 Professional with Service Pack 4 and Update Rollup 1
* Windows XP Professional with Service Pack 3

Tested on default virtual sound card (ES1371) using both VMware Tools driver and Creative Sound Blaster AudioPCI WDM driver (`SBPCI_WebDrvsV5_12_01.exe`).

Other VMware products and versions that uses the same hypervisor should also work.

For other Windows 2000/XP guest configurations, the service should work as well (but not guaranteed).

## Usage
1. Download VMXAudioFix.exe and place it into a stable location
2. Run it. An icon will show up in notification area of the taskbar

## Development Prerequisites
To avoid introducing external dependencies (including redistributables), the following development platforms and tools are used:

* Visual C++ 6.0
* Microsoft Platform SDK, February 2003 Edition
* Microsoft DirectX 7.0 SDK

Newer versions of the above could also be used with some modifications.

Before the build, generate-build-number.sh needs to be executed in a Git / Bash shell to generate build information code file (`BuildInfo.h`).
