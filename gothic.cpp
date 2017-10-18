#include <windows.h>
#include <sstream>
#include <map>
#include "gothic.hpp"
#include "main.hpp"
#include "scripts.hpp"
#include "events.hpp"

DWORD pausedTime = 0;
bool isPaused = false;

void __thiscall hookZenGamePause(CGame *self) {
    fdebug();
    pausedTime = GetTickCount();
    isPaused = true;
    Game::Pause(self);
    return;
}

void __thiscall hookZenGameUnpause(CGame *self) {
    fdebug();
    DWORD pausedDiff = GetTickCount() - pausedTime;

    std::map<int, Script::asynchronousTask>::iterator it = Script::timeout_tasks.begin();
    while(it != Script::timeout_tasks.end()) {
        it->second.time += pausedDiff;
        it++;
    }

    isPaused = false;
    Game::Unpause(self);
    return;
}

void __thiscall hookDefineExternalsUlfi(CGame* self, CParser* parser) {
    debug("hookDefineExternalsUlfi():: executing original");
    Game::DefineExternalsUlfi(self, parser);
    debug("hookDefineExternalsUlfi():: making custom definitions");
    Script::registerExternals(parser);
    debug("hookDefineExternalsUlfi():: done!");
    return;
}

BOOL WINAPI hookPeekMessage(LPMSG lpMsg, HWND hWND, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
    if (!isPaused) {
        Script::processTimeoutTasks();
    }
    #ifdef ENABLE_NETWORKING
    Events::processNetEvents();
    #endif
    return PeekMessage(lpMsg, hWND, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

void* __thiscall hookMaterialConstructor(CMaterial* self, const ZString& name) {
    debug("hookMaterialConstructor(" << (std::string)name << ")");
    void* retval = Material::Constructor(self, name);
    fdebug();
    return retval;
}

void __thiscall hookParserReset(CParser* self) {
    debug("hookParserReset()");
    if (Parser::instance == self) {
        Script::timeout_tasks.clear();
    }
    debug("hookParserReset():: calling original");
    Parser::Reset(self);
}

void __cdecl hookHandleCheat(ZString text) {
    debug("hookHandleCheat()");
    if (!Script::disableCheats) {
        Console::HandleCheat(text);
    }
}

bool __thiscall hookWorldSaveWorld(CWorld* world, ZString& name, int e, int x, int y) {
    debug("hookWorldSaveWorld():: " << (std::string)name << " " << e << " " << x << " " << y);
    bool result = World::SaveWorld(world, name, e, x, y);
    debug("-> " << result);
    return result;
}

//--- PATCHERY ---
void patchCheats() {
    fdebug();
    //inlineHook(0x432EC0, 0x432EC7, Console::HandleCheat, hookHandleCheat); //HandleResultString
    //patch(0x47EC65 + 1, DWORD(hookHandleCheat) - (0x47EC65 + 5));
}

void patchEnginePause() {
    fdebug();
    patch(0x83C2E8, hookZenGamePause);
    patch(0x83C2EC, hookZenGameUnpause);
    inlineHook(0x793100, 0x793106, Parser::Reset, hookParserReset);
}

void patchHandleEvent() { //patchuje event handler gry
    fdebug();
    //inlineHook(0x6B8000, 0x6B8006, NPC::SetBodyState, hookSetBodyState);
    //inlineHook(0x6A5F50, 0x6A5F51, NPC::RemoveFromSlot, hookRemoveFromSlot);
}

void patchPeekMessage() { //hookuje PeekMessage z WinAPI
    fdebug();
    // patchuje idata (import PeekMessageEx)
    patch(0x82E3D8, hookPeekMessage);
}

void patchDefineExternalsUlfi() {
    fdebug();
    // patchuje offset jedynego skoku do DefineExternalsUlfi
    // nadpisując go tak, żeby kierował do naszego hooka
    patch(0x6C4C57, DWORD(hookDefineExternalsUlfi) - (0x6C4C56 + 5));
}

void researchPatch() {
    fdebug();
    patch(0x83E188, hookWorldSaveWorld);
    //inlineHook(0x563EE0, 0x563EE2, Material::Constructor, hookMaterialConstructor);
}
