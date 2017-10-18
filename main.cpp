#include "main.hpp"
#include "core.hpp"
#include "gothic.hpp"

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <iostream>
#ifdef DEBUG
    #include <fstream>
#endif
#include <queue>
#include <sstream>

#ifdef DEBUG
std::ofstream logger;
#endif

bool init(HINSTANCE dllHandle) {
    #ifdef DEBUG
    logger.open("gtools.log");

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout.sync_with_stdio();
    std::cout << "gtools - G2NK Scripting Toolchain" << std::endl;
    std::cout << "by Demonical Monk, SoulFire" << std::endl;
    std::cout << "debug compilation - testing purposes only" << std::endl;
    Sleep(1500);
    std::cout << std::endl;
    #endif

    debug("init():: Starting with DLL instance " << dllHandle << ".");

    debug("Patching various things...");
    //patchCheats();
    patchEnginePause();
    patchPeekMessage();
    patchDefineExternalsUlfi();
    patchHandleEvent();
    researchPatch();
    debug("Patching done!");

    return true;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            return init(hinstDLL);
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
