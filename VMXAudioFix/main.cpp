#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "constant.h"
#include "setup.h"
#include "service.h"
#include "helper.h"
#include "dxmusic.h"

int main(int argc, char* argv[])
{
	puts("VMware Audio Fix Service");
	puts("(C) 2021 Xlfdll Workstation");
	puts("");

    int rVal = EXIT_SUCCESS;

	if (argc > 1)
	{
		if (strncmp(argv[1], "/?", 2) == 0)
		{
			puts("/install - Install service into the system");
			puts("/uninstall - Remove service from the system");
		}
        else if (strncmp(argv[1], "/install", 8) == 0)
        {
            puts("Installing service...");
            puts("");

            if (!InstallService())
            {
                puts("WARNING: Service installation failed.");

                rVal = EXIT_FAILURE;
            }
        }
        else if (strncmp(argv[1], "/uninstall", 10) == 0)
        {
            puts("Uninstalling service...");
            puts("");

            if (!UninstallService())
            {
                puts("WARNING: Service uninstallation failed.");

                rVal = EXIT_FAILURE;
            }
        }

        puts("");
	}
	else
	{
		// Must call CoInitialize(NULL) here to initialize COM
		// (even if it was called in DirectMusic initialization function)
		// Otherwise access violation error will occur
		CoInitialize(NULL);

        ServiceDebugOutDWORD
            (TEXT("[VMXAudioFixService] Starting...\n"), 0);

		SERVICE_TABLE_ENTRY dispatchTable[] = 
		{
			{ VMXAUDIOFIX_SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain },
			{ NULL, NULL }
		};

		if (!StartServiceCtrlDispatcher(dispatchTable))
		{
			ServiceDebugOutDWORD
				(TEXT("[VMXAudioFixService] StartServiceCtrlDispatcher() failed (%lu).\n"),
				GetLastError());

            rVal = EXIT_FAILURE;
		}

		CoUninitialize();
	}

	return rVal;
}