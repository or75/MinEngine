#pragma once

#include <Windows.h>

#include "MinEngine/MinEngine.h"

struct GlobalVar
{
	ApiTable	Api;
	DWORD_PTR	DeltaOffset;
};

PVOID operator new ( size_t size );
void operator delete( PVOID lpDst , size_t size );

DWORD_PTR WINAPI MainThread( PVOID lpThreadParam );
GlobalVar* GetGlobalVar();
PVOID GetReturn();