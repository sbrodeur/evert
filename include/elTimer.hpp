#ifndef __ELTIMER_HPP
#define __ELTIMER_HPP
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

#if !defined (__ELDEFS_HPP)
#	include "elDefs.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Timer
{
public:
			Timer	(void);

	void	clear	(void);
	double	get		(void) const;

private:
	int64_t m_timer;
};

class CPUTimer
{
public:
	EL_FORCE_INLINE			CPUTimer	(void)			{ m_timer = 0; }
	
	EL_FORCE_INLINE	void		clear		(void)			{ m_timer = 0; }
	EL_FORCE_INLINE void	toggle		(void)			{ m_timer = rdtsc() - m_timer; }
	EL_FORCE_INLINE int64_t	getI		(void) const	{ return m_timer; }
	EL_FORCE_INLINE double	get			(void) const	{ return (double)getI() * s_msec; }

	static int64_t			rdtsc		(void);
	void					calibrate	(const Timer& referenceTimer);

private:
	int64_t					m_timer;
	static double			s_msec;		// calibration counter
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELTIMER_HPP
