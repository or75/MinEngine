#include "Main.h"

int WINAPI Entry( LPVOID lpThreadParam )
{
	MODULE_User32 User32Dll;

	if ( InitMinEngine( MainThread , 0x10 ) )
	{
		if ( User32Dll.Load() )
		{
			auto pGlobal = GetGlobalVar();

			char STR_Entry[] = { 'E', 'n', 't', 'r', 'y', 0 };
			char STR_EntryTitle[] = { 'E', 'n', 't', 'r', 'y', 'T', 'i', 't', 'l', 'e', 0 };

			if ( pGlobal->Api.MessageBoxA )
			{
				pGlobal->Api.MessageBoxA( 0 , STR_Entry , STR_EntryTitle , MB_ICONINFORMATION );
			}

			MinZeroStr( STR_Entry );
			MinZeroStr( STR_EntryTitle );
		}
	}

	return 0;
}

DWORD_PTR WINAPI MainThread( PVOID lpThreadParam )
{
	auto pGlobal = (GlobalVar*)lpThreadParam;

	while ( pGlobal )
	{
		char STR_csx_CheatEntry[] = { '[', 'c', 's', 'x', ']', ' ', 'C', 'h', 'e', 'a', 't', 'E', 'n', 't', 'r', 'y', 0 };
		
		pGlobal->Api.fnOutputDebugStringA( STR_csx_CheatEntry );
		
		MinZeroStr( STR_csx_CheatEntry );

		pGlobal->Api.fnSleep( 2500 );
	}
	
	return 0;
}