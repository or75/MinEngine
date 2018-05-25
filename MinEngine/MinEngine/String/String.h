#pragma once

#include "../MinEngine.h"

namespace MinEngine
{
	bool isupper( char c );
	char tolower( char c );

	UINT strlen( const char* lpStr1 );
	int strcmpi( const char* lpStr1 , const char* lpStr2 );
	char* strcat( const char*strDest , const char* strSource );
}