#ifndef __ELPATHSOLUTION_HPP
#define __ELPATHSOLUTION_HPP
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

class Beam;
class Listener;
class Polygon;
class Room;
class Source;

struct Path
{
	int							m_order;
	std::vector<Vector3>		m_points;
	std::vector<const Polygon*>	m_polygons;
};


struct SolutionNode
{
public:
	int				m_parent;
	const Polygon*	m_polygon;
};

class PathSolution
{
public:

								PathSolution	(const Room& room,
												 const Source& source,
												 const Listener& listener,
												 int maximumOrder);
								~PathSolution	(void);

	void						update			(void);

	int							numPaths		(void) const			{ return m_paths.size(); }
	const Path&					getPath			(int i) const			{ EL_ASSERT(i >= 0 && i < numPaths()); return m_paths[i]; }
	void						renderPath		(const Path& path) const;
private:
								PathSolution	(const PathSolution&);	// prohibit
	const PathSolution&			operator=		(const PathSolution&);	// prohibit

	void						solveRecursive	(const Vector3& source,
												 const Vector3& target,
												 const Beam& beam,
												 int order,
												 int parentIndex);

	void						validatePath	(const Vector3& source,
												 const Vector3& target,
												 int nodeIndex,
												 Vector4& failPlane);

	static Vector4				getFailPlane	(const Beam& beam,
												 const Vector3& target);

	void						clearCache		(void);

	const Room&					m_room;
	const Source&				m_source;
	const Listener&				m_listener;
	int							m_maximumOrder;

	std::vector<const Polygon*>	m_polygonCache;
	std::vector<Vector3>		m_validateCache;
	std::multimap<float, int>	m_pathFirstSet;

	std::vector<SolutionNode>	m_solutionNodes;
	std::vector<Vector4>		m_failPlanes;
	std::vector<Vector4>		m_distanceSkipCache;
	Vector3						m_cachedSource;

	std::vector<Path>			m_paths;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELPATHSOLUTION_HPP
