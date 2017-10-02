#ifndef __ELBEAM_HPP
#define __ELBEAM_HPP
/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#if !defined (__ELPOLYGON_HPP)
#	include "elPolygon.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Beam
{
public:
							Beam			(void);
							Beam			(const Vector3& top, const Polygon& polygon);
							Beam			(const Beam& beam);
							~Beam			(void);

	const Beam&				operator=		(const Beam& beam);

	const Vector3&			getTop			(void) const	{ return m_top; }
	const Polygon&			getPolygon		(void) const	{ return m_polygon; }
	int						numPleqs		(void) const	{ return (int)m_pleqs.size(); }
	const Vector4&			getPleq			(int i) const	{ EL_ASSERT(i >= 0 && i < numPleqs()); return m_pleqs[i]; }

	void					render			(const Vector3& color) const;
	EL_FORCE_INLINE bool	contains		(const Vector3& p) const
	{
		for (int i=0; i < numPleqs(); i++)
			if (dot(p, getPleq(i)) < 0.f)
				return false;
		return true;
	}

private:
	void					calculatePleqs	(void);

	Vector3					m_top;
	Polygon					m_polygon;
	std::vector<Vector4>	m_pleqs;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELBEAM_HPP
