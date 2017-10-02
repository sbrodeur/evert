/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elBeam.hpp"

#include <GL/glu.h>

using namespace EL;

//------------------------------------------------------------------------

Beam::Beam(void)
{
	// empty
}

Beam::Beam(const Vector3& top, const Polygon& polygon)
:	m_top	 (top),
	m_polygon(polygon)
{
	calculatePleqs();
}

Beam::Beam(const Beam& beam)
:	m_top	 (beam.m_top),
	m_polygon(beam.m_polygon),
	m_pleqs	 (beam.m_pleqs)
{
	// empty
}

Beam::~Beam(void)
{
	// empty
}

const Beam& Beam::operator=(const Beam& beam)
{
	m_top = beam.m_top;
	m_polygon = beam.m_polygon;
	m_pleqs = beam.m_pleqs;
	return *this;
}

//------------------------------------------------------------------------

void Beam::calculatePleqs(void)
{
	int n = m_polygon.numPoints();

	m_pleqs.resize(n + 1);
	Vector3 p1 = m_polygon[n-1];

	float sign = dot(m_top, m_polygon.getPleq()) > 0.f ? -1.f : 1.f;

	for (int i=0; i < n; i++)
	{
		Vector3 p0 = p1;
		p1 = m_polygon[i];
		m_pleqs[i+1] = sign * normalize(getPlaneEquation(m_top, p0, p1));
	}
	m_pleqs[0] = sign * m_polygon.getPleq();
}

void Beam::render(const Vector3& color) const
{
	static const float extrudeDist = 100000.f;	// 100 m

	if (m_polygon.numPoints() == 0)
		return;

	glColor3fv(&color.x);
	glBegin(GL_LINE_LOOP);
	for (int i=0; i < m_polygon.numPoints(); i++)
		glVertex3fv(&m_polygon[i].x);
	glEnd();

	glBegin(GL_LINES);
	for (int i=0; i < m_polygon.numPoints(); i++)
	{
		Vector3 p = m_polygon[i];
		glVertex3fv(&p.x);
		p += extrudeDist * normalize(p-m_top);
		glVertex3fv(&p.x);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i=0; i < m_polygon.numPoints(); i++)
		glVertex3fv(&m_polygon[i].x);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i=0; i <= m_polygon.numPoints(); i++)
	{
		Vector3 p = m_polygon[i % m_polygon.numPoints()];
		Vector3 e = p + extrudeDist * normalize(p-m_top);
		glVertex3fv(&p.x);
		glVertex3fv(&e.x);
	}
	glEnd();
}
