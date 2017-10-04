/******************************************************************************
 *
 * Copyright (c) 2004-2005, Samuli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  - Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "elOrientedPoint.hpp"
#include "elGLUT.hpp"

#include <GL/gl.h>
#include <GL/glut.h>

using namespace EL;

//------------------------------------------------------------------------

OrientedPoint::OrientedPoint(void)
:	m_position(0.f, 0.f, 0.f)
{
	// empty
}

OrientedPoint::OrientedPoint(const OrientedPoint& s)
:	m_position		(s.m_position),
	m_orientation	(s.m_orientation),
	m_name			(s.m_name)
{
	// empty
}

OrientedPoint::~OrientedPoint(void)
{
	// empty
}

const OrientedPoint& OrientedPoint::operator=(const OrientedPoint& s)
{
	m_position = s.m_position;
	m_orientation = s.m_orientation;
	m_name = s.m_name;
	return *this;
}

//------------------------------------------------------------------------

void OrientedPoint::render(const Vector3& color) const
{
	const Vector3& pos  = getPosition();
	Vector3		   posd = pos + getDirection() * 1000.f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.f);
	glPointSize(10.f);
	glBegin(GL_POINTS);
	glColor3fv(&color.x);
	glVertex3fv(&pos.x);
	glEnd();
	glDisable(GL_ALPHA_TEST);

	glBegin(GL_LINES);
	glVertex3fv(&pos.x);
	glVertex3fv(&posd.x);
	glEnd();

	const char* string = getName().c_str();

	glPushMatrix();

	// determine string width
	glLoadIdentity();
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	for (const char* p = string; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	float width = m[12];

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glPopMatrix();
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	Matrix3x4 orient;
	orient.setRotation(getOrientation());
	GLUT::multMatrix(orient);
	glScalef(4.f, 4.f, 4.f);
	glTranslatef(-width/2, -40.f, 200.f);

	static const float miny = -30.f;
	static const float maxy = 110.f;
	static const float pad	= 20.f;

	Vector3 cv[5] =
	{
		Vector3(-pad, miny-pad, 0.f),
		Vector3(width+pad, miny-pad, 0.f),
		Vector3(width+pad, maxy+pad, 0.f),
		Vector3(-pad, maxy+pad, 0.f),
		Vector3(width/2, 40.f, -200.f)
	};

	glColor4f(.5f, .5f, .5f, .5f);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	for (int j=0; j < 4; j++)
		glVertex3fv(&cv[j].x);
	glEnd();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	for (int j=0; j < 4; j++)
	{
		glVertex3fv(&cv[j].x);
		glVertex3fv(&cv[(j+1)%4].x);
		glVertex3fv(&cv[j].x);
		glVertex3fv(&cv[4].x);
	}
	glEnd();

	glColor3f(1,1,1);
	glLineWidth(2.f);
	for (const char* p = string; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);

	glPopMatrix();
	glPopAttrib();
}
