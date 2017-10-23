#ifndef __ELBSP_HPP
#define __ELBSP_HPP
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
#if !defined (__ELDEFS_HPP)
#	include "elDefs.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Beam;
class Polygon;
class Ray;
class BSP
{
public:
					BSP						(void);
					~BSP					(void);

	void			constructHierarchy		(const Polygon** polygons, int numPolygons);

	void			beamCast				(const Beam& beam, std::vector<const Polygon*>& result) const;
	const Polygon*	rayCast					(const Ray& ray) const;
	bool			rayCastAny				(const Ray& ray) const;
	static Vector3	getIntersectionPoint	(void);

	class TempNode;
private:
	TempNode*		m_hierarchy;
	uintptr_t*		m_list;
	AABB			m_aabb;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELBSP_HPP
