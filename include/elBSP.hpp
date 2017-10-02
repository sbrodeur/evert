#ifndef __ELBSP_HPP
#define __ELBSP_HPP
/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#if !defined (__ELAABB_HPP)
#	include "elAABB.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
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
	intptr_t*	m_list;
	AABB			m_aabb;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELBSP_HPP
