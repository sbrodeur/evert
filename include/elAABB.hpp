#ifndef __ELAABB_HPP
#define __ELAABB_HPP
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

#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class AABB
{
public:
	Vector3						m_mn;
	Vector3						m_mx;

	EL_FORCE_INLINE 			AABB		(void) : m_mn(0.f, 0.f, 0.f), m_mx(0.f, 0.f, 0.f)	{}
	EL_FORCE_INLINE 			AABB		(const Vector3& mn, const Vector3& mx) : m_mn(mn), m_mx(mx) {}
	EL_FORCE_INLINE 			AABB		(const AABB& aabb) : m_mn(aabb.m_mn), m_mx(aabb.m_mx) {}
	EL_FORCE_INLINE 			~AABB		(void) {}
	EL_FORCE_INLINE const AABB&	operator=	(const AABB& aabb) { m_mn = aabb.m_mn; m_mx = aabb.m_mx; }

	EL_FORCE_INLINE void		grow		(const Vector3& p)
	{
		for (int j=0; j < 3; j++)
		{
			if (p[j] < m_mn[j]) m_mn[j] = p[j];
			if (p[j] > m_mx[j]) m_mx[j] = p[j];
		}
	}
	EL_FORCE_INLINE bool		overlaps	(const AABB& o) const
	{
		return (m_mn.x < o.m_mx.x && m_mx.x > o.m_mn.x &&
				m_mn.y < o.m_mx.y && m_mx.y > o.m_mn.y &&
				m_mn.z < o.m_mx.z && m_mx.z > o.m_mn.z);
	}
	EL_FORCE_INLINE bool		contains	(const Vector3& p) const
	{
		return (p.x > m_mn.x && p.x < m_mx.x &&
				p.y > m_mn.y && p.y < m_mx.y &&
				p.z > m_mn.z && p.z < m_mx.z);
	}
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELAABB_HPP
