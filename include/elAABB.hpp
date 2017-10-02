#ifndef __ELAABB_HPP
#define __ELAABB_HPP
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
