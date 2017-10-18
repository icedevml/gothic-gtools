#pragma once

#include "core.hpp"
#include "gothic_zenstring.hpp"

class oCItem;

typedef void* (*OCITEM_CONSTRUCTOR)(oCItem*);
typedef void* (*OCITEM_CONSTRUCTOR2)(oCItem*, ZString&, int);
//zSTRING *__thiscall(oCItem *self, zSTRING *string_placeholder, int costam);
typedef ZString* __thiscall (*OCITEM_GET_NAME)(oCItem*, ZString*, int);

DEFINE_FUNCTION(OCITEM_CONSTRUCTOR, oCItemConstructor, 0x670DE0);
DEFINE_FUNCTION(OCITEM_CONSTRUCTOR2, oCItemConstructor2, 0x670FA0);
DEFINE_FUNCTION(OCITEM_GET_NAME, oCItemGetName, 0x672170);

class oCItem {
    char padding1[716];
    int count[6]; //offset 716 @24
    char padding2[4];
    int instanceid; //offset 744 @4

    oCItem() {
        oCItemConstructor(this);
    }

    oCItem(ZString& arg1, int arg2) {
        oCItemConstructor2(this, arg1, arg2);
    }

    std::string GetName(int);
};
