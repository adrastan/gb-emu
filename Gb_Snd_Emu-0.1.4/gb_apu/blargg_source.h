
// By default, #included at beginning of library source files

// Copyright (C) 2005 Shay Green.

#ifndef BLARGG_SOURCE_H
#define BLARGG_SOURCE_H

// If debugging is enabled, abort program if expr is false. Meant for checking
// internal state and consistency. A failed assertion indicates a bug in the module.
// void assert( bool expr );
#include <assert.h>

// If debugging is enabled and expr is false, abort program. Meant for checking
// caller-supplied parameters and operations that are outside the control of the
// module. A failed requirement indicates a bug outside the module.
// void require( bool expr );
#undef require
#define require( expr ) assert(( "unmet requirement", expr ))

// Like printf() except output goes to debug log file. Might be defined to do
// nothing (not even evaluate its arguments).
// void dprintf( const char* format, ... );
#undef dprintf
#define dprintf (1) ? ((void) 0) : (void)

// If enabled, evaluate expr and if false, make debug log entry with source file
// and line. Meant for finding situations that should be examined further, but that
// don't indicate a problem. In all cases, execution continues normally.
#undef check
#define check( expr ) ((void) 0)

// If expr returns non-NULL error string, return it from current function, otherwise continue.
#define BLARGG_RETURN_ERR( expr ) do {                          \
		blargg_err_t blargg_return_err_ = (expr);               \
		if ( blargg_return_err_ ) return blargg_return_err_;    \
	} while ( 0 )

// If ptr is NULL, return out of memory error string.
#define BLARGG_CHECK_ALLOC( ptr )   do { if ( !(ptr) ) return "Out of memory"; } while ( 0 )

// Avoid any macros which evaluate their arguments multiple times
#undef min
#undef max

// using const references generates crappy code, and I am currenly only using these
// for built-in types, so they take arguments by value

template<class T>
inline T min( T x, T y )
{
	if ( x < y )
		return x;
	return y;
}

template<class T>
inline T max( T x, T y )
{
	if ( x < y )
		return y;
	return x;
}

#endif

