#include <string>
#include "scripts.hpp"
#include "gothic.hpp"
#include "main.hpp"

namespace Script {
    int task_counter = 0;
    bool isConsoleInitialized = false;

    //
    // externals
    //
    int __cdecl _setTimeout() {
        task_counter++;
        int arg;
        Parser::GetParameterInt(Parser::instance, arg);
        int time;
        Parser::GetParameterInt(Parser::instance, time);
        ZString tmp;
        Parser::GetParameterStr(Parser::instance, tmp);
        std::string func(tmp);
        debug("_setTimeout(" << func << ", " << time << ", " << arg << ")");
        asynchronousTask task;
        task.func = func;
        task.time = GetTickCount() + (DWORD)time;
        task.withArgument = 1;
        task.argument = arg;
        timeout_tasks.insert(std::pair<int, asynchronousTask>(task_counter, task));
        debug("_setTimeout():: return " << task_counter);
        Parser::SetReturnInt(Parser::instance, task_counter);
        return 1;
    }

    int __cdecl _clearTimeout() {
        int index;
        Parser::GetParameterInt(Parser::instance, index);
        debug("_clearTimeout(" << index << ")");
        if (timeout_tasks.erase(index) > 0) {
            Parser::SetReturnInt(Parser::instance, 1);
        } else {
            Parser::SetReturnInt(Parser::instance, 0);
        }
        return 1;
    }

    int __cdecl _debugLog() {
        ZString tmp;
        Parser::GetParameterStr(Parser::instance, tmp);
        std::string text(tmp);
        debug("_debugLog():: " << text);
        return 1;
    }

    int __cdecl _execCommand() {
        fdebug();
        if (!isConsoleInitialized) {
            Game::ConsoleInit();
            isConsoleInitialized = true;
        }
        ZString text;
        Parser::GetParameterStr(Parser::instance, text);
        int result = Console::Evaluate(Console::instance, text);
        debug("Console::Evaluate(instance, " << (std::string)text << ") = " << result);
        Parser::SetReturnInt(Parser::instance, result);
        return 1;
    }

    int __cdecl _equipWeapon() {
        fdebug();
        void* item = Parser::GetParameterObj(Parser::instance);
        void* npc = Parser::GetParameterObj(Parser::instance);
        NPC::EquipWeapon((CNpc*)npc, (CItem*)item);
        return 1;
    }

    int __cdecl _castPtr() {
        fdebug();
        int addr;
        Parser::GetParameterInt(Parser::instance, addr);
        Parser::SetReturnObj(Parser::instance, (void*)addr);
        return 1;
    }

    int __cdecl _readByte() {
        fdebug();
        int addr;
        Parser::GetParameterInt(Parser::instance, addr);
        Parser::SetReturnInt(Parser::instance, (int)*(char*)addr);
        return 1;
    }

    int __cdecl _readInt() {
        fdebug();
        int addr;
        Parser::GetParameterInt(Parser::instance, addr);
        Parser::SetReturnInt(Parser::instance, *(int*)addr);
        return 1;
    }

    int __cdecl _writeByte() {
        fdebug();
        int addr;
        int val;
        Parser::GetParameterInt(Parser::instance, val);
        Parser::GetParameterInt(Parser::instance, addr);
        *(char*)addr = (char)val;
        return 1;
    }

    int __cdecl _writeInt() {
        fdebug();
        int addr;
        int val;
        Parser::GetParameterInt(Parser::instance, val);
        Parser::GetParameterInt(Parser::instance, addr);
        *(int*)addr = (int)val;
        return 1;
    }

    int __cdecl _setMotionFactor() {
        int tmp;
        Parser::GetParameterInt(Parser::instance, tmp);
        float factor = tmp / 1000;
        Timer::SetMotionFactor(Timer::instance, factor);
        return 1;
    }

    int _cdecl _getTime() {
        int day, hour, minute = 0;
        Game::GetTime(Game::instance, day, hour, minute);
        Parser::SetReturnInt(Parser::instance, hour*60+minute);
        return 1;
    }

    //
    // public
    //
    void registerExternals(CParser* parser) {
        // console related
        Parser::DefineExternal(parser, "gtl_execCommand", _execCommand, ZEN_INT, ZEN_STRING, ZEN_VOID);

        // timers
        Parser::DefineExternal(parser, "gtl_setTimeout", _setTimeout, ZEN_INT, ZEN_STRING, ZEN_INT, ZEN_INT, ZEN_VOID);
        Parser::DefineExternal(parser, "gtl_clearTimeout", _clearTimeout, ZEN_INT, ZEN_INT, ZEN_VOID);

        // hax
        Parser::DefineExternal(parser, "gtl_equipWeapon", _equipWeapon, ZEN_VOID, ZEN_INSTANCE, ZEN_INSTANCE, ZEN_VOID);
        Parser::DefineExternal(parser, "gtl_setMotionFactor", _setMotionFactor, ZEN_VOID, ZEN_INT, ZEN_VOID);

        // "reflection"
        Parser::DefineExternal(parser, "gtl_castPtr", _castPtr, ZEN_INSTANCE, ZEN_INT, ZEN_VOID);

        // raw memory
        Parser::DefineExternal(parser, "gtl_readInt", _readInt, ZEN_INT, ZEN_INT, ZEN_VOID);
        Parser::DefineExternal(parser, "gtl_readByte", _readByte, ZEN_INT, ZEN_INT, ZEN_VOID);
        Parser::DefineExternal(parser, "gtl_writeInt", _writeInt, ZEN_VOID, ZEN_INT, ZEN_INT, ZEN_VOID);
        Parser::DefineExternal(parser, "gtl_writeByte", _writeByte, ZEN_VOID, ZEN_INT, ZEN_INT, ZEN_VOID);

        // tools
        Parser::DefineExternal(parser, "gtl_getTime", _getTime, ZEN_INT, ZEN_VOID);

        // debug
        Parser::DefineExternal(parser, "gtl_debug", _debugLog, ZEN_VOID, ZEN_STRING, ZEN_VOID);
    }

    void processTimeoutTasks() {
        if (!Script::timeout_tasks.empty()) {
            unsigned long current_time = (unsigned long)GetTickCount();
            std::map<int, Script::asynchronousTask>::iterator it = Script::timeout_tasks.begin();

            while(it != Script::timeout_tasks.end()) {
                if (it->second.time <= current_time) {
                    debug(__FUNCTION__ << "():: Looking up for function index.");
                    int index = Parser::GetIndex(Parser::instance, it->second.func.c_str());
                    if (index > 0) {
                        debug(__FUNCTION__ << "():: Calling function " << index);
                        Parser::CallFunc(Parser::instance, index, it->second.argument);
                    }
                    debug(__FUNCTION__ << "():: Erasing entry.");
                    Script::timeout_tasks.erase(it++);
                } else {
                    ++it;
                }
            }
        }
    }
}
