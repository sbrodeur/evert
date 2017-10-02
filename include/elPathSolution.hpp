#ifndef __ELPATHSOLUTION_HPP
#define __ELPATHSOLUTION_HPP
/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

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

class PathSolution
{
public:
	struct Path
	{
		int							m_order;
		std::vector<Vector3>		m_points;
		std::vector<const Polygon*>	m_polygons;
	};


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

	struct SolutionNode;

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
