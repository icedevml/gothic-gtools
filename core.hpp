#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include "main.hpp"

#define DEFINE_FUNCTION(type, name, addr) const type name = (type)addr
#ifdef DEBUG
    #define fdebug()  debug(__FUNCTION__ << "():: entered function");
    #define debug(text)   std::cout << text << std::endl; logger << text << std::endl;
#else
    #define fdebug()  ;
    #define debug(text)  ;
#endif

#define PROD_MODE 1
#define DEV_MODE 2
#define UNSUPPORTED_MODE -1

template <class T>
void patch(ULONG_PTR addr, T value) {
    DWORD oldProtect = 0;
    VirtualProtect(LPVOID(addr), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
    *(T *)addr = value;
    VirtualProtect(LPVOID(addr), sizeof(T), oldProtect, NULL);
}

template <typename T>
void inlineHook(ULONG_PTR start, ULONG_PTR end, T& originalFunc, T hookFunc) {
    static BYTE* buffer = (BYTE*)VirtualAlloc(NULL, 69, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    SIZE_T size = end - start;
    SIZE_T proxy_size = size + 5;
    BYTE *memory = buffer;
    BYTE *jump = memory + size;
    buffer += proxy_size;
    memcpy((PVOID)memory, (PVOID)start, size);

    *jump = 0xE9;
    *(DWORD *)(jump + 1) = end - ((DWORD)jump + 5);

    patch(start, BYTE(0xE9));
    patch(start + 1, DWORD(hookFunc) - (start + 5));

    originalFunc = (T)memory;
}
