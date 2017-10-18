#ifndef GOTHIC_HPP
#define GOTHIC_HPP

#include "core.hpp"
#include "gothic_zenstring.hpp"

class CGame;
class CGameManager;
class CParser;
class ZError;
class CEventMessage;
class CVob;
class CNpc;
class CInput;
class CAniCtrl;
class CMaterial;
class CConsole;
class CWorld;
class CItem;
class CTimer;
class CView;

void patchPeekMessage();
void patchDefineExternalsUlfi();
void patchDefineExternalsUlfi_Spacer();
void patchHandleEvent();
void patchPeekMessage();
void patchEnginePause();
void researchPatch();

class CEventMessage {
   public:
        BYTE unknown_1[36];
        WORD eventType;
};

class VEC {
    public:
        float x;
        float y;
        float z;
        float w;

        VEC(float& _x, float& _y, float& _z) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
        }
        VEC(float& _x, float& _y, float& _z, float _w) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }
};

namespace Parser {
    CParser *const instance = (CParser *const)0xAB40C0;

    typedef CParser* (*GET_PARSER)();
    typedef void (__thiscall *RESET)(CParser *);

    typedef void* (*CALL_FUNC)(CParser *, int, ...);
    typedef void* (*DEFINE_EXTERNAL)(CParser *, const ZString &, int (*)(void), int, ...);
    typedef int (__thiscall *GET_INDEX)(CParser *, const ZString &);

    typedef int (__thiscall *SET_INSTANCE)(CParser *, const ZString &, void *);
    typedef int (__thiscall *SET_INSTANCE_INT)(CParser *, int, void *);

    typedef void (__thiscall *GET_PARAMETER_INT)(CParser *, int &);
    typedef void (__thiscall *GET_PARAMETER_FLOAT)(CParser *, float &);
    typedef void (__thiscall *GET_PARAMETER_STR)(CParser *, ZString &);
    typedef void* (__thiscall *GET_PARAMETER_OBJ)(CParser *);

    typedef void (__thiscall *SET_RETURN_INT)(CParser *, int);
    typedef void (__thiscall *SET_RETURN_FLOAT)(CParser *, float);
    typedef void (__thiscall *SET_RETURN_STR)(CParser *, const ZString &);
    typedef void (__thiscall *SET_RETURN_OBJ)(CParser *, void *);

    typedef void* (__thiscall *GET_SYMBOL_INT)(CParser *, int);
    typedef void* (__thiscall *GET_SYMBOL)(CParser *, const ZString &);

    DEFINE_FUNCTION(GET_PARSER, GetParser, 0x78DEC0);
    static RESET Reset;

    DEFINE_FUNCTION(CALL_FUNC, CallFunc, 0x7929F0);
    static DEFINE_EXTERNAL DefineExternal = (DEFINE_EXTERNAL)0x7A0190;
    DEFINE_FUNCTION(GET_INDEX, GetIndex, 0x793470);

    DEFINE_FUNCTION(SET_INSTANCE, SetInstance, 0x794870);
    DEFINE_FUNCTION(SET_INSTANCE_INT, SetInstanceInt, 0x794910);

    // wrapowane przez funkcje niżej
    DEFINE_FUNCTION(GET_PARAMETER_INT, GetParameterInt, 0x7A0760); // Stos FILO
    DEFINE_FUNCTION(GET_PARAMETER_FLOAT, GetParameterFloat, 0x7A0770);
    DEFINE_FUNCTION(GET_PARAMETER_STR, GetParameterStr, 0x7A07B0);
    DEFINE_FUNCTION(GET_PARAMETER_OBJ, GetParameterObj, 0x7A08F0);

    DEFINE_FUNCTION(SET_RETURN_INT, SetReturnInt, 0x7A0960);
    DEFINE_FUNCTION(SET_RETURN_FLOAT, SetReturnFloat, 0x7A0980);
    DEFINE_FUNCTION(SET_RETURN_STR, SetReturnStr, 0x7A09D0);
    DEFINE_FUNCTION(SET_RETURN_OBJ, SetReturnObj, 0x7A09A0);

    DEFINE_FUNCTION(GET_SYMBOL_INT, GetSymbolInt, 0x7938C0);
    DEFINE_FUNCTION(GET_SYMBOL, GetSymbol, 0x7938D0);
}

namespace Par_Symbol {
    typedef void (__thiscall *SET_OFFSET)(void *, int);
    typedef int (__thiscall *GET_OFFSET)(void *);

    DEFINE_FUNCTION(SET_OFFSET, SetOffset, 0x7A2F20);
    DEFINE_FUNCTION(GET_OFFSET, GetOffset, 0x7A2F30);
}

namespace Game {
    CGame *const instance = (CGame *const) 0xAB0884;

    typedef void (__thiscall *DEFINE_EXTERNALS_ULFI)(CGame *, CParser *);
    typedef int (__thiscall *HANDLE_EVENT)(CGame *, int);
    typedef void (__cdecl *CONSOLE_INIT)();
    typedef void (__thiscall *PAUSE)(CGame *);
    typedef void (__thiscall *UNPAUSE)(CGame *);
    typedef void (__thiscall *GET_TIME)(CGame *, int&, int&, int&);
    typedef void (__thiscall *OPEN_SAVESCREEN)(CGame *, bool);
    typedef void (__thiscall *CLOSE_SAVESCREEN)(CGame *);

    static DEFINE_EXTERNALS_ULFI DefineExternalsUlfi = (DEFINE_EXTERNALS_ULFI)0x6D4780;
    DEFINE_FUNCTION(CONSOLE_INIT, ConsoleInit, 0x6D01F0);
    DEFINE_FUNCTION(PAUSE, Pause, 0x6C8AD0);
    DEFINE_FUNCTION(UNPAUSE, Unpause, 0x6C8D50);
    DEFINE_FUNCTION(GET_TIME, GetTime, 0x6C4E70);
    DEFINE_FUNCTION(OPEN_SAVESCREEN, OpenSavescreen, 0x6C2250);
    DEFINE_FUNCTION(CLOSE_SAVESCREEN, CloseSavescreen, 0x6C2630);
}

namespace View {
    CView *const instance = (CView *const) 0xAB6468;

    typedef int (__thiscall *PRINTCXY)(CView*, ZString&);

    DEFINE_FUNCTION(PRINTCXY, PrintCXY, 0x7AA030);
}

namespace World {
    typedef bool (__thiscall *SAVE_WORLD)(CWorld *, ZString&, int, int, int);

    DEFINE_FUNCTION(SAVE_WORLD, SaveWorld, 0x77FA50);
}

namespace Timer {
    CTimer *const instance = (CTimer *const) 0x99B3D4;

    typedef void (__thiscall *SET_MOTION_FACTOR)(CTimer *, float);

    DEFINE_FUNCTION(SET_MOTION_FACTOR, SetMotionFactor, 0x6370B0);
}

namespace Material {
    typedef void* (__thiscall *MATERIAL_CONSTRUCTOR)(CMaterial *, const ZString &);

    static MATERIAL_CONSTRUCTOR Constructor;
}

namespace Console {
    CConsole *const instance = (CConsole *const) 0xAB3860;

    typedef int (__thiscall *EVALUATE)(CConsole *, const ZString &);
    typedef void (__cdecl *HANDLE_CHEAT)(ZString);

    DEFINE_FUNCTION(EVALUATE, Evaluate, 0x784860);
    DEFINE_FUNCTION(HANDLE_CHEAT, HandleCheat, 0x432EC0);
}

namespace NPC {
    namespace Player {
        static CNpc *const & instance = *(CNpc **)0xAB2684;
    }

    typedef void (__thiscall *EQUIP_WEAPON)(CNpc*, CItem*);

    DEFINE_FUNCTION(EQUIP_WEAPON, EquipWeapon, 0x73A030);

    enum BODY_STATES {
        BS_IDLE = 0,
        BS_WALKING = 1,
        BS_RUNNING = 3,
        BS_JUMPING = 8,
        BS_CLIMBING = 9,
        BS_FALLING = 10,
        BS_SITTING = 11,
        BS_CHEST_OPEN = 13,
        BS_WHEEL = 15,
        BS_USING = 16, //schylenie się nad skrzynią, podejście do drzwi, spanie na lozku, etc
        BS_DEAD = 26,
        BS_UNKNOWN = 127
    };
}

namespace VOB {
    typedef void (__thiscall *GET_POSITION_WORLD)(CVob *, float &, float &, float &);
    typedef void* (__thiscall *SET_POSITION_WORLD)(CVob *, const VEC &);

    DEFINE_FUNCTION(GET_POSITION_WORLD, GetPositionWorld, 0x61B890);
    DEFINE_FUNCTION(SET_POSITION_WORLD, SetPositionWorld, 0x61BB70);
}

namespace Error {
    ZError  *const instance = (ZError *const)  0x8CDCD0;

    typedef int (__thiscall *WARNING)(ZError *, const ZString &);

    DEFINE_FUNCTION(WARNING, Warning, 0x44DA50);
}

namespace Sys {
    typedef void (*SYS_HARD_EXIT)(const char *, ...);

    DEFINE_FUNCTION(SYS_HARD_EXIT, SysHardExit, 0x504A80);
}

//--- ENUMS ---
enum ARGUMENT_TYPE {
    ZEN_VOID = 0,
    ZEN_FLOAT = 1,
    ZEN_INT = 2,
    ZEN_STRING = 3,
    ZEN_FUNC = 5,
    ZEN_INSTANCE = 7
};

#endif //GOTHIC_HPP
