#ifndef __ELLISTENER_HPP
#define __ELLISTENER_HPP
/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#if !defined (__ELORIENTEDPOINT_HPP)
#	include "elOrientedPoint.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Listener : public OrientedPoint
{
public:
						Listener	(void);
						Listener	(const Listener& s);
						~Listener	(void);

	const Listener&		operator=	(const Listener& s);
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELLISTENER_HPP
