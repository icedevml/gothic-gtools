#pragma once

#include "core.hpp"

class ZString;

typedef void* (__thiscall *ZSTRING_CONSTRUCTOR)(ZString *, const char *);
typedef void* (__thiscall *ZSTRING_CLONE)(ZString *, const ZString &);
typedef void* (__thiscall *ZSTRING_DESTRUCTOR)(ZString *);
typedef char* (__thiscall *ZSTRING_TOCHAR)(const ZString *);

DEFINE_FUNCTION(ZSTRING_CONSTRUCTOR, ZStringConstructor, 0x4010C0);
DEFINE_FUNCTION(ZSTRING_CLONE, ZStringClone, 0x416500);
DEFINE_FUNCTION(ZSTRING_DESTRUCTOR, ZStringDestructor, 0x401160);
DEFINE_FUNCTION(ZSTRING_TOCHAR, ZStringToChar, 0x4639D0);

class ZString {
    char internal[20];
    public:
    ZString(const char *str = "") {
        ZStringConstructor(this, str);
    }
    ZString(const ZString& obj) {
        ZStringClone(this, obj);
    }
    ~ZString() {
        ZStringDestructor(this);
    }
    char* ToChar() {
        return ZStringToChar(this);
    }
    operator std::string() const {
        return ZStringToChar(this);
    }
};
