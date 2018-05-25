#pragma once

#include "../MinEngine.h"

#define MinZeroStr(Buff) \
	MinEngine::memset( &Buff , 0 , sizeof( Buff ) );

namespace MinEngine
{
	void memset( PVOID lpDst , char sym , size_t len );
	void memcpy( PVOID lpDst , PVOID lpSrc , DWORD dwCount );

	DWORD_PTR FindPattern( const char* pPattern , const char* pszMask , DWORD_PTR dwStart , DWORD dwSize , DWORD dwOffset );
}