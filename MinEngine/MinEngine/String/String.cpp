#include "String.h"

namespace MinEngine
{
	bool isupper( char c )
	{
		return ( 'A' <= c ) && ( c <= 'Z' );
	}

	char tolower( char c )
	{
		return isupper( c ) ? c - 'A' + 'a' : c;
	}

	UINT strlen( const char* lpStr1 )
	{
		UINT i = 0;
		while ( lpStr1[i] != '\0' )
			i++;
		return i;
	}

	int strcmpi( const char* lpStr1 , const char* lpStr2 )
	{
		int  v;
		char c1 , c2;
		do
		{
			c1 = *lpStr1++;
			c2 = *lpStr2++;
			v = (UINT)tolower( c1 ) - (UINT)tolower( c2 );
		} while ( ( v == 0 ) && ( c1 != '\0' ) && ( c2 != '\0' ) );
		return v;
	}

	char* strcat( char* strDest , const char* strSource )
	{
		char* d = strDest;
		const char* s = strSource;
		while ( *d ) d++;
		do { *d++ = *s++; } while ( *s );
		*d = '\0';
		return strDest;
	}

	char* strcpy( char *strDest , const char *strSource )
	{
		unsigned i;
		for ( i = 0; strSource[i] != '\0'; ++i )
			strDest[i] = strSource[i];
		strDest[i] = '\0';
		return strDest;
	}
}