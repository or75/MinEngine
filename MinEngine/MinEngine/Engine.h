#pragma once

#pragma comment(linker,"/ENTRY:Entry")
#pragma comment(linker,"/NODEFAULTLIB")

#pragma comment(linker,"/MERGE:.text=.code")
#pragma comment(linker,"/SECTION:.code,rwe")

#ifdef _WIN64
#pragma comment(linker,"/BASE:0x100000000")
#define BASE_ADDRESS 0x100001000
#define GLOBAL_OFFSET 2
#define DEAD_CODE 0xADDEADDEADDEADDE
#else
#pragma comment(linker,"/BASE:0x10000000")
#define GLOBAL_OFFSET 4
#define BASE_ADDRESS 0x10001000
#define DEAD_CODE 0xADDEADDE
#endif

#include "MinEngine.h"

struct GlobalVar
{
	ApiTable	Api;
	DWORD_PTR	DeltaOffset;
	PVOID		UserGlobal;
};

// Alloc , Delete , Init
PVOID operator new ( size_t size );
void operator delete( PVOID lpDst , size_t size );

bool InitMinEngine( PVOID StartThread , DWORD dwUserGlobalSize );

// Global Variable
GlobalVar* GetGlobalVar();

// Modules
class MODULE_User32
{
public:
	bool Load();

	HMODULE hUser32 = nullptr;
};