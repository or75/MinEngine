#include "Api.h"

namespace MinEngine
{
	bool Api::Init( ApiTable* pApiTable )
	{
		HMODULE hKernel32 = GetKernel32();

		if ( !hKernel32 )
			return false;

		_GetProcAddress fnGetProcAddress = GetProcAddressFunc( hKernel32 );

		if ( !fnGetProcAddress )
			return false;

		_LoadLibraryA fnLoadLibraryA = 0;
		_GetModuleHandleA fnGetModuleHandleA = 0;
		_GetProcessHeap fnGetProcessHeap = 0;
		_HeapAlloc fnHeapAlloc = 0;
		_HeapFree fnHeapFree = 0;
		_OutputDebugStringA fnOutputDebugStringA = 0;
		_CreateThread fnCreateThread = 0;
		_VirtualQuery fnVirtualQuery = 0;
		_Sleep fnSleep = 0;

		char STR_LoadLibraryA[] = { 'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', 0 };
		char STR_GetModuleHandleA[] = { 'G', 'e', 't', 'M', 'o', 'd', 'u', 'l', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 'A', 0 };	
		char STR_GetProcessHeap[] = { 'G', 'e', 't', 'P', 'r', 'o', 'c', 'e', 's', 's', 'H', 'e', 'a', 'p', 0 };
		char STR_HeapAlloc[] = { 'H', 'e', 'a', 'p', 'A', 'l', 'l', 'o', 'c', 0 };
		char STR_HeapFree[] = { 'H', 'e', 'a', 'p', 'F', 'r', 'e', 'e', 0 };
		char STR_OutputDebugStringA[] = { 'O', 'u', 't', 'p', 'u', 't', 'D', 'e', 'b', 'u', 'g', 'S','t','r','i','n','g','A', 0 };
		char STR_CreateThread[] = { 'C', 'r', 'e', 'a', 't', 'e', 'T', 'h', 'r', 'e', 'a', 'd', 0 };
		char STR_VirtualQuery[] = { 'V', 'i', 'r', 't', 'u', 'a', 'l', 'Q', 'u', 'e', 'r', 'y', 0 };
		char STR_Sleep[] = { 'S', 'l', 'e', 'e', 'p', 0 };

		fnLoadLibraryA = (_LoadLibraryA)fnGetProcAddress( hKernel32 , STR_LoadLibraryA );
		fnGetModuleHandleA = (_GetModuleHandleA)fnGetProcAddress( hKernel32 , STR_GetModuleHandleA );	
		fnGetProcessHeap = (_GetProcessHeap)fnGetProcAddress( hKernel32 , STR_GetProcessHeap );
		fnHeapAlloc = (_HeapAlloc)fnGetProcAddress( hKernel32 , STR_HeapAlloc );
		fnHeapFree = (_HeapFree)fnGetProcAddress( hKernel32 , STR_HeapFree );
		fnOutputDebugStringA = (_OutputDebugStringA)fnGetProcAddress( hKernel32 , STR_OutputDebugStringA );
		fnCreateThread = (_CreateThread)fnGetProcAddress( hKernel32 , STR_CreateThread );
		fnVirtualQuery = (_VirtualQuery)fnGetProcAddress( hKernel32 , STR_VirtualQuery );
		fnSleep = (_Sleep)fnGetProcAddress( hKernel32 , STR_Sleep );

		MinZeroStr( STR_LoadLibraryA );
		MinZeroStr( STR_GetModuleHandleA );
		MinZeroStr( STR_LoadLibraryA );
		MinZeroStr( STR_GetProcessHeap );
		MinZeroStr( STR_HeapAlloc );
		MinZeroStr( STR_HeapFree );
		MinZeroStr( STR_OutputDebugStringA );
		MinZeroStr( STR_CreateThread );
		MinZeroStr( STR_VirtualQuery );
		MinZeroStr( STR_Sleep );

		pApiTable->fnGetProcAddress = fnGetProcAddress;
		pApiTable->fnGetModuleHandleA = fnGetModuleHandleA;
		pApiTable->fnLoadLibraryA = fnLoadLibraryA;	
		pApiTable->fnGetProcessHeap = fnGetProcessHeap;
		pApiTable->fnHeapAlloc = fnHeapAlloc;
		pApiTable->fnHeapFree = fnHeapFree;
		pApiTable->fnOutputDebugStringA = fnOutputDebugStringA;
		pApiTable->fnCreateThread = fnCreateThread;
		pApiTable->fnVirtualQuery = fnVirtualQuery;
		pApiTable->fnSleep = fnSleep;

		return true;
	}

	HMODULE Api::GetKernel32()
	{
		struct LDR_MODULE
		{
			LIST_ENTRY InLoadOrderLinks;
			LIST_ENTRY InMemoryOrderLinks;
			LIST_ENTRY InInitializationOrderLinks;
			HMODULE DllBase;
			PVOID EntryPoint;
			ULONG SizeOfImage;
			UNICODE_STRING FullDllName;
			UNICODE_STRING BaseDllName;
		};

		HMODULE hKernel32 = 0;

#ifdef _WIN64
		PPEB lpPeb = (PPEB)__readgsqword( 0x60 );
#else
		PPEB lpPeb = (PPEB)__readfsdword( 0x30 );
#endif

		PLIST_ENTRY pListHead = &lpPeb->Ldr->InMemoryOrderModuleList;
		PLIST_ENTRY pListEntry = pListHead->Flink;

		while ( pListEntry != pListHead )
		{
			LDR_MODULE* pModEntry = CONTAINING_RECORD( pListEntry , LDR_MODULE , InMemoryOrderLinks );

			if ( pModEntry->FullDllName.Length > 25 )
			{
				PCHAR pDllName = (PCHAR)pModEntry->FullDllName.Buffer;
				USHORT DllLen = pModEntry->FullDllName.Length;

				if ( pDllName[DllLen - 24] == 'K' && pDllName[DllLen - 12] == '3' && pDllName[DllLen - 10] == '2' )
				{
					hKernel32 = pModEntry->DllBase;
					break;
				}
			}

			pListEntry = pListEntry->Flink;
		}

		return hKernel32;
	}

	_GetProcAddress Api::GetProcAddressFunc( HANDLE hKernel32 )
	{
		LPBYTE lpBaseAddr = (LPBYTE)hKernel32;
		PIMAGE_DOS_HEADER lpDosHdr = (PIMAGE_DOS_HEADER)lpBaseAddr;
		PIMAGE_NT_HEADERS pNtHdrs = (PIMAGE_NT_HEADERS)( lpBaseAddr + lpDosHdr->e_lfanew );
		PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)( lpBaseAddr + pNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );

		LPDWORD pNameArray = (LPDWORD)( lpBaseAddr + pExportDir->AddressOfNames );
		LPDWORD pAddrArray = (LPDWORD)( lpBaseAddr + pExportDir->AddressOfFunctions );
		LPWORD pOrdArray = (LPWORD)( lpBaseAddr + pExportDir->AddressOfNameOrdinals );

		char strGetProcAddress[] = { 'G', 'e', 't', 'P', 'r', 'o', 'c', 'A', 'd', 'd', 'r', 'e', 's', 's', 0 };

		for ( UINT i = 0; i < pExportDir->NumberOfNames; i++ )
		{
			LPSTR pFuncName = (LPSTR)( lpBaseAddr + pNameArray[i] );

			if ( !strcmpi( pFuncName , strGetProcAddress ) )
			{
				MinZeroStr( strGetProcAddress );

				return ( _GetProcAddress )( lpBaseAddr + pAddrArray[pOrdArray[i]] );
			}
		}

		return 0;
	}
}