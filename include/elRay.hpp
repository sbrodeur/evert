#ifndef __ELRAY_HPP
#define __ELRAY_HPP
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

#if !defined (__ELPOLYGON_HPP)
#	include "elPolygon.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Ray
{
public:
	Vector3		m_a;
	Vector3		m_b;

	EL_FORCE_INLINE 			Ray			(void)					{}
	EL_FORCE_INLINE 			Ray			(const Vector3& a, const Vector3& b) : m_a(a), m_b(b) {}
	EL_FORCE_INLINE 			Ray			(const Ray& ray)		: m_a(ray.m_a), m_b(ray.m_b) {}
	EL_FORCE_INLINE 			~Ray		(void)					{}
	EL_FORCE_INLINE const Ray&	operator=	(const Ray& ray)		{ m_a = ray.m_a; m_b = ray.m_b; return *this; }

	void						render		(const Vector3& color) const;

	EL_FORCE_INLINE bool		intersect	(const Polygon& polygon) const
	{
		float s0 = dot(m_a, polygon.getPleq());
		float s1 = dot(m_b, polygon.getPleq());

		if (s0*s1 >= 0.f)
			return false;

		int n = polygon.numPoints();

		Vector3 dir = m_b - m_a;
		Vector3 eb  = polygon[n-1] - m_a;
		float sign = 0.f;
		for (int i=0; i < n; i++)
		{
			Vector3 ea = eb;
			eb = polygon[i] - m_a;

			float det = dot(dir, cross(ea, eb));

			if (sign == 0.f)
				sign = det;
			else if (det * sign < 0.f)
				return false;
		}

		return (sign != 0.f);
	}

	EL_FORCE_INLINE bool		intersectExt(const Polygon& polygon) const
	{
		int n = polygon.numPoints();

		Vector3 dir = m_b - m_a;
		Vector3 eb  = polygon[n-1] - m_a;
		float sign = 0.f;
		for (int i=0; i < n; i++)
		{
			Vector3 ea = eb;
			eb = polygon[i] - m_a;

			float det = dot(dir, cross(ea, eb));

			if (sign == 0.f)
				sign = det;
			else if (det * sign < 0.f)
				return false;
		}

		return (sign != 0.f);
	}
};

EL_FORCE_INLINE Vector3 intersect(const Ray& ray, const Vector4& pleq)
{
	float s0 = dot(ray.m_a, pleq);
	float s1 = dot(ray.m_b, pleq);

	return ray.m_a + (s0/(s0-s1)) * (ray.m_b - ray.m_a);
}

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELRAY_HPP
