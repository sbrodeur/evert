#ifndef __ELSOURCE_HPP
#define __ELSOURCE_HPP
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

class Source : public OrientedPoint
{
public:
						Source		(void);
						Source		(const Source& s);
						~Source		(void);

	const Source&		operator=	(const Source& s);
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELSOURCE_HPP
