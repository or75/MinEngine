#include "Engine.h"

PVOID operator new ( size_t size )
{
	auto pGlobal = GetGlobalVar();

	if ( (DWORD_PTR)pGlobal == DEAD_CODE )
		return nullptr;

	return pGlobal->Api.fnHeapAlloc( pGlobal->Api.fnGetProcessHeap() , HEAP_ZERO_MEMORY , size );
}

void operator delete( PVOID lpDst , size_t size )
{
	auto pGlobal = GetGlobalVar();

	if ( (DWORD_PTR)pGlobal == DEAD_CODE )
		return;

	pGlobal->Api.fnHeapFree( pGlobal->Api.fnGetProcessHeap() , HEAP_ZERO_MEMORY , lpDst );
}

GlobalVar* GetGlobalVar()
{
#ifdef _WIN64
	return (GlobalVar*)DEAD_CODE;
#else
	return (GlobalVar*)DEAD_CODE;
#endif
}

bool InitMinEngine( PVOID StartThread , DWORD dwUserGlobalSize )
{
	GlobalVar LocalGlobal;

	if ( MinEngine::Api::Init( &LocalGlobal.Api ) )
	{
#ifndef _WIN64
		MEMORY_BASIC_INFORMATION MemoryBasicInfo;

		LocalGlobal.Api.fnVirtualQuery( GetReturnAddress() , &MemoryBasicInfo , sizeof( MEMORY_BASIC_INFORMATION ) );

		PVOID BaseAddress = MemoryBasicInfo.BaseAddress;
		DWORD_PTR Delta = (DWORD_PTR)BaseAddress - (DWORD_PTR)BASE_ADDRESS;

		LocalGlobal.DeltaOffset = Delta;
#else
		LocalGlobal.DeltaOffset = 0;
#endif
		PVOID pGlobalVar = LocalGlobal.Api.fnHeapAlloc( LocalGlobal.Api.fnGetProcessHeap() , HEAP_ZERO_MEMORY , sizeof GlobalVar );
		PVOID pUserGlobal = LocalGlobal.Api.fnHeapAlloc( LocalGlobal.Api.fnGetProcessHeap() , HEAP_ZERO_MEMORY , dwUserGlobalSize );

		if ( pGlobalVar && pUserGlobal )
		{
			LocalGlobal.UserGlobal = pUserGlobal;

			MinEngine::memcpy( pGlobalVar , &LocalGlobal , sizeof GlobalVar );

#ifdef _WIN64
			*(PDWORD_PTR)( (DWORD_PTR)GetGlobalVar + GLOBAL_OFFSET ) = (DWORD_PTR)pGlobalVar;
#else
			*(PDWORD_PTR)( (DWORD_PTR)GetGlobalVar + LocalGlobal.DeltaOffset + GLOBAL_OFFSET ) = (DWORD_PTR)pGlobalVar;
#endif
			if ( StartThread )
			{
				LocalGlobal.Api.fnCreateThread( 0 , 0 , (LPTHREAD_START_ROUTINE)( (DWORD_PTR)StartThread + LocalGlobal.DeltaOffset ) , pGlobalVar , 0 , 0 );
			}

			return true;
		}
	}

	return false;
}

bool MODULE_User32::Load()
{
	auto pGlobal = GetGlobalVar();

	if ( (DWORD_PTR)pGlobal == DEAD_CODE )
		return false;

	if ( pGlobal->Api.MessageBoxA == nullptr )
	{
		char STR_User32[] = { 'U', 's', 'e', 'r', '3', '2', 0 };
		char STR_MessageBoxA[] = { 'M', 'e', 's', 's', 'a', 'g', 'e', 'B', 'o', 'x', 'A', 0 };

		hUser32 = pGlobal->Api.fnGetModuleHandleA( STR_User32 );

		if ( hUser32 == nullptr )
		{
			hUser32 = pGlobal->Api.fnLoadLibraryA( STR_User32 );

			if ( hUser32 )
			{
				pGlobal->Api.MessageBoxA = (_MessageBoxA)pGlobal->Api.fnGetProcAddress( hUser32 , STR_MessageBoxA );
			}
		}

		MinEngine::memset( STR_User32 , 0 , MinEngine::strlen( STR_User32 ) );
		MinEngine::memset( STR_MessageBoxA , 0 , MinEngine::strlen( STR_MessageBoxA ) );
	}

	return true;
}