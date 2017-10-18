#pragma once
#include <map>
#include <windows.h>
#include "gothic.hpp"

namespace Script {
    struct asynchronousTask {
        std::string func;
        DWORD time;
        bool withArgument;
        int argument;
    };

    static std::map<int, asynchronousTask> timeout_tasks;
    static bool disableCheats = false;

    void registerExternals(CParser* parser);
    void processTimeoutTasks();
};
