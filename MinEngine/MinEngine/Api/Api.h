#pragma once

#include "../MinEngine.h"

using _GetProcAddress = FARPROC( WINAPI* )( HMODULE , LPCSTR );
using _GetModuleHandleA = HMODULE( WINAPI* )( LPCSTR );
using _LoadLibraryA = HMODULE( WINAPI* )( LPCSTR );
using _GetProcessHeap = HANDLE( WINAPI* )( );
using _HeapAlloc = LPVOID( WINAPI* )( HANDLE , DWORD , SIZE_T );
using _HeapFree = BOOL( WINAPI* )( HANDLE , DWORD , LPVOID );
using _OutputDebugStringA = void( WINAPI* )( LPCSTR );
using _CreateThread = HANDLE( WINAPI* ) ( LPSECURITY_ATTRIBUTES , SIZE_T , LPTHREAD_START_ROUTINE , LPVOID , DWORD , LPDWORD );
using _VirtualQuery = SIZE_T( WINAPI* ) ( LPCVOID , PMEMORY_BASIC_INFORMATION , SIZE_T );
using _Sleep = void( WINAPI* )( DWORD );

using _MessageBoxA = int ( WINAPI* )( HWND , LPCTSTR , LPCTSTR , UINT );

struct ApiTable
{
	//Kernel32
	_GetProcAddress fnGetProcAddress = 0;
	_LoadLibraryA fnLoadLibraryA = 0;
	_GetModuleHandleA fnGetModuleHandleA = 0;
	_GetProcessHeap fnGetProcessHeap = 0;
	_HeapAlloc fnHeapAlloc = 0;
	_HeapFree fnHeapFree = 0;
	_OutputDebugStringA fnOutputDebugStringA = 0;
	_CreateThread fnCreateThread = 0;
	_VirtualQuery fnVirtualQuery = 0;
	_Sleep fnSleep = 0;
	//User32
	_MessageBoxA MessageBoxA;
};

namespace MinEngine
{
	class Api
	{
	public:
		static bool Init( ApiTable* pApiTable );

	private:
		static HMODULE GetKernel32();
		static _GetProcAddress GetProcAddressFunc( HANDLE hKernel32 );
	};
}