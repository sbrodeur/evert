/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elSource.hpp"

using namespace EL;

//------------------------------------------------------------------------

Source::Source(void)
{
	// empty
}

Source::Source(const Source& s)
:	OrientedPoint(s)
{
	// empty
}

Source::~Source(void)
{
	// empty
}

const EL::Source& Source::operator=(const Source& s)
{
	OrientedPoint::operator=(s);
	return *this;
}

//------------------------------------------------------------------------

