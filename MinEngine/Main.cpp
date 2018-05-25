#include "Main.h"

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

int WINAPI Entry( LPVOID lpThreadParam )
{
	GlobalVar LocalGlobal;

	if ( MinEngine::Api::Init( &LocalGlobal.Api ) )
	{	
#ifndef _WIN64
		MEMORY_BASIC_INFORMATION MemoryBasicInfo;

		LocalGlobal.Api.fnVirtualQuery( GetReturn() , &MemoryBasicInfo , sizeof( MEMORY_BASIC_INFORMATION ) );

		PVOID BaseAddress = MemoryBasicInfo.BaseAddress;
		DWORD_PTR Delta = (DWORD_PTR)BaseAddress - (DWORD_PTR)BASE_ADDRESS;

		LocalGlobal.DeltaOffset = Delta;
#else
		LocalGlobal.DeltaOffset = 0;
#endif
		PVOID pGlobalVar = LocalGlobal.Api.fnHeapAlloc( LocalGlobal.Api.fnGetProcessHeap() , HEAP_ZERO_MEMORY , sizeof GlobalVar );

		if ( pGlobalVar )
		{
			MinEngine::memcpy( pGlobalVar , &LocalGlobal , sizeof GlobalVar );

#ifdef _WIN64
			*(PDWORD_PTR)( (DWORD_PTR)GetGlobalVar + GLOBAL_OFFSET ) = (DWORD_PTR)pGlobalVar;
#else
			*(PDWORD_PTR)( (DWORD_PTR)GetGlobalVar + Delta + GLOBAL_OFFSET ) = (DWORD_PTR)pGlobalVar;
#endif

			LocalGlobal.Api.fnCreateThread( 0 , 0 , (LPTHREAD_START_ROUTINE)( (DWORD_PTR)MainThread + LocalGlobal.DeltaOffset ) , 0 , 0 , 0 );
		}
		
	}

	return 0;
}

DWORD_PTR WINAPI MainThread( PVOID lpThreadParam )
{
	auto pGlobal = GetGlobalVar();

	char STR_User32[] = { 'U', 's', 'e', 'r', '3', '2', 0 };
	char STR_MessageBoxA[] = { 'M', 'e', 's', 's', 'a', 'g', 'e', 'B', 'o', 'x', 'A', 0 };

	HMODULE hUser32 =  pGlobal->Api.fnLoadLibraryA( STR_User32 );

	if ( hUser32 )
	{
		pGlobal->Api.MessageBoxA = (_MessageBoxA)pGlobal->Api.fnGetProcAddress( hUser32 , STR_MessageBoxA );

		if ( pGlobal->Api.MessageBoxA )
		{
			pGlobal->Api.MessageBoxA( 0 , STR_MessageBoxA , STR_User32 , MB_ICONINFORMATION );
		}
	}

	MinZeroStr( STR_User32 );
	MinZeroStr( STR_MessageBoxA );

	return 0;
}

GlobalVar* GetGlobalVar()
{
#ifdef _WIN64
	return (GlobalVar*)DEAD_CODE;
#else
	return (GlobalVar*)DEAD_CODE;
#endif
}

PVOID GetReturn()
{
	return _ReturnAddress();
}
