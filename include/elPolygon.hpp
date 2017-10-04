#ifndef __ELPOLYGON_HPP
#define __ELPOLYGON_HPP
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

#if !defined (__ELAABB_HPP)
#	include "elAABB.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class AABB;
class Beam;
class Polygon
{
public:
									Polygon		(void);
									Polygon		(const Polygon& p);
									Polygon		(const Vector3* points, int numPoints);
									Polygon		(const Vector3* points, int numPoints, const Vector4& pleq);
									Polygon		(const std::vector<Vector3>& points);
									~Polygon	(void);

	const Polygon&					operator=	(const Polygon& p);

	int								numPoints	(void) const  { return m_points.size(); }
	EL_FORCE_INLINE Vector3&		operator[]	(int i)		  { EL_ASSERT(i >= 0 && i < (int)m_points.size()); return m_points[i]; }
	EL_FORCE_INLINE const Vector3&	operator[]	(int i) const { EL_ASSERT(i >= 0 && i < (int)m_points.size()); return m_points[i]; }
	const Vector4&					getPleq		(void) const  { return m_pleq;   }
	const Vector3&					getNormal	(void) const  { return *((const Vector3*)&m_pleq); }
	bool							isConvex	(void) const;
	float							getNonPlanarity(void) const;
	float							getArea		(void) const;
	AABB							getAABB		(void) const;

	void							triangulate	(std::vector<Polygon>& triangles);
	void							splitConvex	(std::vector<Polygon>& polygons);

	enum ClipResult
	{
		CLIP_CLIPPED,
		CLIP_ORIGINAL,
		CLIP_VANISHED
	};

	ClipResult						clip		(const Vector4& pleq);
	ClipResult						clip		(const AABB& aabb);
	ClipResult						clip		(const Beam& beam);

	void							render		(const Vector3& color) const;

private:
	void							calculatePleq(void);
	static bool						mergePartials(std::vector<std::vector<int> >& partials, const std::vector<Vector3>& vertices);

	static ClipResult				clipInner	(const Vector3* inPoints, int numInPoints,
												 Vector3* outPoints, int& numOutPoints,
												 const Vector4& pleq);

	static std::vector<Vector3>		s_clipBuffer[2];
	std::vector<Vector3>			m_points;
	Vector4							m_pleq;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELPOLYGON_HPP
