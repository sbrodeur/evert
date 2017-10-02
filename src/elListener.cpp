/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elListener.hpp"

using namespace EL;

//------------------------------------------------------------------------

Listener::Listener(void)
{
	// empty
}

Listener::Listener(const Listener& s)
:	OrientedPoint(s)
{
	// empty
}

Listener::~Listener(void)
{
	// empty
}

const EL::Listener& Listener::operator=(const Listener& s)
{
	OrientedPoint::operator=(s);
	return *this;
}

//------------------------------------------------------------------------

