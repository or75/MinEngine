#include "Memory.h"
/*
PVOID operator new ( size_t size )
{
	return HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , size );
}

void operator delete( PVOID lpDst )
{
	HeapFree( GetProcessHeap() , 0 , lpDst );
}
*/
namespace MinEngine
{
	void memset( PVOID lpDst , char sym , size_t len )
	{
		for ( size_t i = 0; i < len; i++ )
			( (PCHAR)lpDst )[i] = sym;
	}

	void memcpy( PVOID lpDst , PVOID lpSrc , DWORD dwCount )
	{
		LPBYTE s = (LPBYTE)lpSrc;
		LPBYTE d = (LPBYTE)lpDst;

		while ( dwCount-- )
			*d++ = *s++;
	}

	DWORD_PTR FindPattern( const char* pPattern , const char* pszMask , DWORD_PTR dwStart , DWORD dwSize , DWORD dwOffset )
	{
		bool bFound = false;

		DWORD dwMaskLen = MinEngine::strlen( pszMask );

		DWORD_PTR dwEnd = dwStart + dwSize - dwMaskLen;

		for ( DWORD_PTR dwPtr = dwStart; dwPtr < dwEnd; dwPtr++ )
		{
			bFound = true;

			for ( DWORD idx = 0; idx < dwMaskLen; idx++ )
			{
				BYTE p = pPattern[idx];
				BYTE s = *(PBYTE)( dwPtr + idx );

				if ( pszMask[idx] == 'x' && p != s )
				{
					bFound = false;
					break;
				}
			}

			if ( bFound )
				return dwPtr + dwOffset;
		}

		return 0;
	}
}