#ifndef __ELDEFS_HPP
#define __ELDEFS_HPP
/******************************************************************************
 *
 * Copyright (c) 2004-2005, Samuli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  - Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// windows.h specialities -- don't define the Polygon struct
//------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#define NOGDI

//------------------------------------------------------------------------
// load standard libraries and disable a couple of warnings in MSVC
//------------------------------------------------------------------------

#if defined(_MSC_VER)
#	pragma warning(disable: 4505)	// unreferenced local function has been removed
#	pragma warning(disable: 4514)	// unreferenced inline function has been removed
#	pragma warning(disable: 4710)	// function x not inlined
#	pragma warning(disable: 4714)	// function x marked as __forceinline not inlined
#	pragma warning(disable: 4786)	// identifier was truncated to n characters in the debug information
#	pragma warning(push, 1)
#endif

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdlib>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

//------------------------------------------------------------------------
// assertions
//------------------------------------------------------------------------

#if defined(EL_DEBUG)
#	include <cassert>
#	define EL_ASSERT(e)		assert(e)
#else
#	define EL_ASSERT(e)		(static_cast<void>(0))					
#endif

//------------------------------------------------------------------------
// debug code
//------------------------------------------------------------------------

#if defined(EL_DEBUG)
#	define EL_DEBUG_CODE(X) X
#else
#	define EL_DEBUG_CODE(X)
#endif

//------------------------------------------------------------------------
// our stuff
//------------------------------------------------------------------------

namespace EL
{

#define	PI						3.141592654f
template<class TT> inline TT	RAD( TT x ) { return x * TT(PI) / TT(180); }
template<class TT> inline TT	DEG( TT x )	{ return x * TT(180) / TT(PI); }

//------------------------------------------------------------------------
// Debug macros
//------------------------------------------------------------------------

#if defined(_DEBUG)
#	undef EL_DEBUG
#	define EL_DEBUG
#endif // _DEBUG

//------------------------------------------------------------------------
// The for() fix macro for MSVC (as Visual C++ does not respect the C++
// standard)
//------------------------------------------------------------------------

#if defined(_MSC_VER)
#	if !defined (for)
		__forceinline bool getFalse(void) { return false; }
#		define for if(::EL::getFalse()); else for
#	endif // for
#endif

//------------------------------------------------------------------------
// Forced inlines
//------------------------------------------------------------------------

#if defined (_MSC_VER)
#	define EL_FORCE_INLINE __forceinline
#else
#	define EL_FORCE_INLINE inline
#endif

//------------------------------------------------------------------------
// Commonly used swap,min2,max2 templates
//------------------------------------------------------------------------

template <class T> inline void swap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;
}

template <class T> inline const T& min2 (const T& a, const T& b)
{
	if(a <= b) return a;
	else return b;
}

template <class T> inline const T& max2 (const T& a, const T& b)
{
	if(a >= b) return a;
	else return b;
}

inline float frand(void)
{
	return (float)rand() / (float)RAND_MAX;
}

//------------------------------------------------------------------------
}	   // namespace EL

#endif // __ELDEFS_HPP
