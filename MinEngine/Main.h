#pragma once

#include <Windows.h>

#include "MinEngine/MinEngine.h"

struct GlobalVar
{
	ApiTable	Api;
	DWORD_PTR	DeltaOffset;
};

DWORD_PTR WINAPI MainThread( PVOID lpThreadParam );
GlobalVar* GetGlobalVar();
PVOID GetReturn();