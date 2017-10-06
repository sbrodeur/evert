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

#include "elPolygon.hpp"
#include "elAABB.hpp"
#include "elBeam.hpp"
#include "elTimer.hpp"

#include <GL/glu.h>

using namespace EL;

CPUTimer t_polyClip;

//------------------------------------------------------------------------

Polygon::Polygon(void)
:	m_pleq(0.f, 0.f, 0.f, 0.f),
	m_materialId(0)
{
	// empty
}

Polygon::Polygon(const Polygon& p)
:	m_points(p.m_points),
	m_pleq	(p.m_pleq),
	m_materialId(p.m_materialId)
{
	// empty
}

Polygon::Polygon(const Vector3* points, int numPoints)
:	m_points(numPoints),
	m_materialId(0)
{
	for (int i=0; i < numPoints; i++)
		m_points[i] = points[i];

	calculatePleq();
}

Polygon::Polygon(const Vector3* points, int numPoints, const Vector4& pleq)
:	m_points(numPoints),
	m_pleq	(pleq),
	m_materialId(0)
{
	for (int i=0; i < numPoints; i++)
		m_points[i] = points[i];
}

Polygon::Polygon(const Vector3* points, int numPoints, const Vector4& pleq, unsigned int materialId)
:	m_points(numPoints),
	m_pleq	(pleq),
	m_materialId(materialId)
{
	for (int i=0; i < numPoints; i++)
		m_points[i] = points[i];
}

Polygon::Polygon(const std::vector<Vector3>& points)
:	m_points(points),
	m_materialId(0)
{
	calculatePleq();
}

Polygon::Polygon(const std::vector<Vector3>& points, unsigned int materialId)
:	m_points(points),
	m_materialId(materialId)
{
	calculatePleq();
}

Polygon::~Polygon(void)
{
	// empty
}

const Polygon& Polygon::operator=(const Polygon& p)
{
	m_points = p.m_points;
	m_pleq   = p.m_pleq;
	m_materialId = p.m_materialId;
	return *this;
}

//------------------------------------------------------------------------

void Polygon::calculatePleq(void)
{
	int n = numPoints();

	// compute aggregate normal
	Vector3 normalSum(0.f, 0.f, 0.f);
	for (int i=0; i < n-2; i++)
	{
		const Vector3& v0 = m_points[0];
		const Vector3& v1 = m_points[i+1];
		const Vector3& v2 = m_points[i+2];

		normalSum += cross(v1-v0, v2-v0);
	}

	// use triangle with largest cross product
	float   bestMagnitude = 0.f;
	m_pleq.set(0.f, 0.f, 0.f, 0.f);

	for (int i=0; i < n-2; i++)
	for (int j=i+1; j < n-1; j++)
	for (int k=j+1; k < n; k++)
	{
		const Vector3& v0 = m_points[i];
		const Vector3& v1 = m_points[j];
		const Vector3& v2 = m_points[k];

		Vector4 pleq = getPlaneEquation(v0, v1, v2);
		float mag = pleq.x*pleq.x + pleq.y*pleq.y + pleq.z*pleq.z;

		if (mag > bestMagnitude)
		{
			bestMagnitude = mag;
			m_pleq = pleq;
		}
	}

	if (bestMagnitude == 0.f)
		return;

	// normalize and correct if flipped normal
	Vector3 normal(m_pleq.x, m_pleq.y, m_pleq.z);
	if (dot(normal, normalSum) < 0.f)
		m_pleq = -m_pleq;
	m_pleq *= 1.f/normal.length();
}

//------------------------------------------------------------------------

static std::vector<Polygon>* s_polyTriangles  = 0;
static std::vector<Vector3>* s_polyVertices   = 0;
static unsigned int			 s_polyMaterialId = 0;
static GLenum				 s_polyMode       = 0;
static EL::Vector4			 s_polyPleq;

static void polyTessBegin(GLenum mode)
{
	s_polyMode = mode;
}

static void polyTessEnd(void)
{
	int n = s_polyVertices->size();
	
	if (s_polyMode == GL_TRIANGLES)
	{
		for (int i=0; i < n; i += 3)
			s_polyTriangles->push_back(Polygon(&(*s_polyVertices)[i], 3, s_polyPleq, s_polyMaterialId));
	}
	else if (s_polyMode == GL_TRIANGLE_FAN)
	{
		Vector3 v[3];
		v[0] = (*s_polyVertices)[0];
		for (int i=0; i < n-2; i++)
		{
			v[1] = (*s_polyVertices)[i+1];
			v[2] = (*s_polyVertices)[i+2];
			s_polyTriangles->push_back(Polygon(v, 3, s_polyPleq, s_polyMaterialId));
		}
	}
	else if (s_polyMode == GL_TRIANGLE_STRIP)
	{
		for (int i=0; i < n-2; i++)
		{
			Vector3 v[3];
			for (int j=0; j < 3; j++)
				v[j] = (*s_polyVertices)[i+j];
			if (i&1)
				swap(v[1], v[2]);
			s_polyTriangles->push_back(Polygon(v, 3, s_polyPleq, s_polyMaterialId));
		}
	}
	else
		EL_ASSERT(0);

	s_polyVertices->resize(0);
}

static void polyTessVertex(const Vector3* v)
{
	s_polyVertices->push_back(*v);
}

void Polygon::triangulate(std::vector<Polygon>& triangles)
{
	std::vector<Vector3> vertices;

	s_polyTriangles  = &triangles;
	s_polyVertices   = &vertices;
	s_polyPleq		 = m_pleq;
	s_polyMaterialId = m_materialId;

	GLUtesselator* tobj = gluNewTess();
	gluTessBeginPolygon(tobj, 0);
	gluTessBeginContour(tobj);

	typedef void NFN(void);

	gluTessCallback(tobj, GLU_TESS_BEGIN,  (NFN*)polyTessBegin);
	gluTessCallback(tobj, GLU_TESS_VERTEX, (NFN*)polyTessVertex);
	gluTessCallback(tobj, GLU_TESS_END,    (NFN*)polyTessEnd);

	for (int i=0; i < numPoints(); i++)
	{
		const Vector3& v = m_points[i];
		double d[3] = { v.x, v.y, v.z };
		gluTessVertex(tobj, d, (void*)(&v));
	}

	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);

	s_polyTriangles = 0;
	s_polyVertices  = 0;
}

//------------------------------------------------------------------------

void Polygon::splitConvex(std::vector<Polygon>& polygons)
{
	std::vector<Polygon> triangles;
	triangulate(triangles);

	// indexed partial polygons
	std::vector<std::vector<int> > partials(triangles.size());

	// weld vertices
	std::vector<Vector3> vertices;
	for (int i=0; i < (int)triangles.size(); i++)
	{
		partials[i].resize(triangles[i].numPoints());
		for (int j=0; j < triangles[i].numPoints(); j++)
		{
			const Vector3& v = triangles[i][j];

			bool found = false;
			for (int k=0; k < (int)vertices.size(); k++)
			{
				if (v == vertices[k])
				{
					partials[i][j] = k;
					found = true;
					break;
				}
			}

			if (!found)
			{
				partials[i][j] = vertices.size();
				vertices.push_back(v);
			}
		}
	}

	// weld as long as convex
	for(;;)
	{
		bool success = mergePartials(partials, vertices);
		if (!success)
			break;
	}

	// reconstruct
	for (int i=0; i < (int)partials.size(); i++)
	{
		int n = partials[i].size();
		std::vector<Vector3> vloop(n);
		for (int j=0; j < n; j++)
			vloop[j] = vertices[partials[i][j]];

		Polygon poly(&vloop[0], vloop.size(), m_pleq, m_materialId);	// force same pleq and material id
		polygons.push_back(poly);
	}
}

//------------------------------------------------------------------------

bool Polygon::mergePartials(std::vector<std::vector<int> >& partials, const std::vector<Vector3>& vertices)
{
	for (int i=0; i < (int)partials.size(); i++)
	{
		int n = partials[i].size();
		for (int j=0; j < n; j++)
		{
			int iv0 = partials[i][j];
			int iv1 = partials[i][(j+1)%n];

			for (int k=0; k < (int)partials.size(); k++)
			{
				if (k==i)
					continue;

				int m = partials[k].size();
				for (int p=0; p < m; p++)
				{
					int jv0 = partials[k][p];
					int jv1 = partials[k][(p+1)%m];

					if (iv0 == jv1 && iv1 == jv0)
					{
						std::vector<int> loop;
						for (int q=0; q < n; q++)
							loop.push_back(partials[i][(j+1+q)%n]);
						for (int q=0; q < m-2; q++)
							loop.push_back(partials[k][(p+2+q)%m]);

						std::vector<Vector3> vloop(loop.size());
						for (int q=0; q < (int)loop.size(); q++)
							vloop[q] = vertices[loop[q]];

						Polygon poly(vloop);
						if (poly.isConvex())	
						{
							partials[i] = loop;
							if (k != (int)partials.size()-1)
								swap(partials[k], partials[partials.size()-1]);
							partials.resize(partials.size()-1);
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------

bool Polygon::isConvex(void) const
{
	// construct 2d basis

	Vector3 n = getNormal();
	Vector3 p = cross(Vector3(1,0,0), n);
	Vector3 q = cross(Vector3(0,1,0), n);

	if (p.lengthSqr() > q.lengthSqr())
		q = cross(n, p);
	else
		p = -cross(n, q);

	p.normalize();
	q.normalize();

	Matrix3 basis;
	basis.setRow(0, p);
	basis.setRow(1, q);
	basis.setRow(2, n);
	basis.invert();

	// check convexity
	int np = numPoints();
	for (int i=0; i < np; i++)
	{
		Vector3 v0 = m_points[i] * basis;
		Vector3 v1 = m_points[(i+1)%np] * basis;
		Vector3 v2 = m_points[(i+2)%np] * basis;

		float x1 = v1.x-v0.x;
		float y1 = v1.y-v0.y;
		float x2 = v2.x-v0.x;
		float y2 = v2.y-v0.y;

		float c = x1*y2-x2*y1;
		if (c < 0.f)
			return false;
	}

	return true;
}

//------------------------------------------------------------------------

float Polygon::getNonPlanarity(void) const
{
	float err = 0.f;
	for (int i=0; i < numPoints(); i++)
		err = max2(err, dot(m_points[i], m_pleq));

	return err;
}

float Polygon::getArea(void) const
{
	int n = numPoints();

	Vector3 sum(0.f, 0.f, 0.f);
	for (int i=0; i < n-2; i++)
	{
		const Vector3& v0 = m_points[0];
		const Vector3& v1 = m_points[i+1];
		const Vector3& v2 = m_points[i+2];

		sum += cross(v1-v0, v2-v0);
	}

	return .5f*sum.length();
}

AABB Polygon::getAABB(void) const
{
	AABB aabb;
	if (!numPoints())
		return aabb;

	aabb.m_mn = aabb.m_mx = m_points[0];
	for (int i=1; i < numPoints(); i++)
		aabb.grow(m_points[i]);

	return aabb;
}

//------------------------------------------------------------------------

std::vector<Vector3> Polygon::s_clipBuffer[2];
EL_FORCE_INLINE Polygon::ClipResult Polygon::clipInner(const Vector3* inPoints, int numInPoints,
													   Vector3* outPoints, int& numOutPoints,
													   const Vector4& pleq)
{
	numOutPoints = 0;
	if (!numInPoints)
		return CLIP_VANISHED;

	t_polyClip.toggle();

	ClipResult result = CLIP_ORIGINAL;
	Vector3 a;
	Vector3 b  = inPoints[numInPoints-1];
	float   sa = 0.f;
	float	sb = dot(b, pleq);

	for (int i=0; i < numInPoints; i++)
	{
		a = b;
		b = inPoints[i];
		sa = sb;
		sb = dot(b, pleq);
		bool na = sa < 0.f;
		bool nb = sb < 0.f;

		if (!na && !nb)
		{
			outPoints[numOutPoints++] = b;
			continue;
		}

		result = CLIP_CLIPPED;
		if (na && nb)
			continue;

		float	cval = sa/(sa-sb);
		Vector3 c	 = a + cval*(b-a);

		outPoints[numOutPoints++] = c;

		if (na)
			outPoints[numOutPoints++] = b;
	}

	t_polyClip.toggle();

	EL_ASSERT(numOutPoints <= numInPoints*2);

	if (numOutPoints == 0)
		return CLIP_VANISHED;
	return result;
}

//------------------------------------------------------------------------

Polygon::ClipResult Polygon::clip(const Vector4& pleq)
{
	int n = m_points.size();

	// static workspace for clipper
	if ((int)s_clipBuffer[0].size() < n*2)
	{
		s_clipBuffer[0].resize(n*2);
		s_clipBuffer[1].resize(n*2);
	}

	int clippedVertexCount;
	ClipResult result = clipInner(
		&m_points[0], m_points.size(),
		&s_clipBuffer[0][0], clippedVertexCount,
		pleq);

	m_points.resize(clippedVertexCount);
	for (int i=0; i < clippedVertexCount; i++)
		m_points[i] = s_clipBuffer[0][i];

	return result;
}

//------------------------------------------------------------------------

Polygon::ClipResult Polygon::clip(const AABB& aabb)
{
	bool clipped = false;

	for (int axis = 0; axis < 3; axis++)
	for (int dir  = 0; dir < 2 ; dir++)
	{
		Vector4 pleq(0.f, 0.f, 0.f, 0.f);
		pleq[axis] = dir ? 1.f : -1.f;
		pleq.w = -pleq[axis] * (dir ? aabb.m_mn[axis] : aabb.m_mx[axis]);

		ClipResult res = clip(pleq);
		if (res == CLIP_VANISHED)
			return CLIP_VANISHED;
		else if (res == CLIP_CLIPPED)
			clipped = true;
	}

	if (clipped)
		return CLIP_CLIPPED;

	return CLIP_ORIGINAL;
}

//------------------------------------------------------------------------

Polygon::ClipResult Polygon::clip(const Beam& beam)
{
	int m = numPoints();
	if (!m)
		return CLIP_VANISHED;

	int n = beam.numPleqs();
	if (!n)
		return CLIP_ORIGINAL;

	ClipResult result = CLIP_ORIGINAL;

	if ((int)s_clipBuffer[0].size() < (n+m)*2)
	{
		s_clipBuffer[0].resize((n+m)*2);
		s_clipBuffer[1].resize((n+m)*2);
	}

	int clippedVertices;
	ClipResult res = clipInner(
		&m_points[0], m_points.size(),
		&s_clipBuffer[0][0], clippedVertices,
		beam.getPleq(0));
	if (res == CLIP_VANISHED)
		return CLIP_VANISHED;
	else if (res == CLIP_CLIPPED)
		result = CLIP_CLIPPED;

	Vector3* clipSource = &s_clipBuffer[0][0];
	Vector3* clipTarget = &s_clipBuffer[1][0];

	for (int i=1; i < n; i++)
	{
		int newClippedVertices;
		ClipResult res = clipInner(
			clipSource, clippedVertices,
			clipTarget, newClippedVertices,
			beam.getPleq(i));

		clippedVertices = newClippedVertices;
		swap(clipSource, clipTarget);

		if (res == CLIP_VANISHED)
			return CLIP_VANISHED;
		else if (res == CLIP_CLIPPED)
			result = CLIP_CLIPPED;
	}

	m_points.resize(clippedVertices);
	for (int i=0; i < clippedVertices; i++)
		m_points[i] = clipSource[i];

	return result;
}

//------------------------------------------------------------------------

void Polygon::render(const Vector3& color) const
{
	glBegin(GL_POLYGON);
	glColor3fv(&color.x);
	for (int i=0; i < numPoints(); i++)
		glVertex3fv(&m_points[i].x);
	glEnd();
}
