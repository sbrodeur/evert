/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elRay.hpp"

#include <GL/glu.h>

using namespace EL;

//------------------------------------------------------------------------

void Ray::render(const Vector3& color) const
{
	glBegin(GL_LINES);
	glColor3fv(&color.x);
	glVertex3fv(&m_a.x);
	glVertex3fv(&m_b.x);
	glEnd();
}
