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

#include "elPathSolution.hpp"
#include "elBeam.hpp"
#include "elBSP.hpp"
#include "elGLUT.hpp"
#include "elListener.hpp"
#include "elRay.hpp"
#include "elRoom.hpp"
#include "elSource.hpp"
#include "elTimer.hpp"

#include <GL/glu.h>
#include <stdio.h>

using namespace EL;

//------------------------------------------------------------------------

CPUTimer t_validation;

static const float EPS_SIMILAR_PATHS			= 1.f;
static const float EPS_DEGENERATE_POLYGON_AREA	= 1.f;
static const int   DISTANCE_SKIP_BUCKET_SIZE	= 16;

//------------------------------------------------------------------------

void PathSolution::renderPath(const Path& path) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	float width = 2.f*(m_maximumOrder-path.m_order+1.f);
	float alpha = ((float)m_maximumOrder-path.m_order+1)/(m_maximumOrder+1);

	glLineWidth(width);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.f, 1.f, 0.f, alpha);

	glBegin(GL_LINE_STRIP);
	for (int i=0; i < (int)path.m_points.size(); i++)
		glVertex3fv(&path.m_points[i].x);
	glEnd();

	glPointSize(width*2.f);
	glColor4f(1.f, 1.f, 1.f, alpha*2);
	glEnable(GL_POINT_SMOOTH);
	glDepthMask(GL_FALSE);
	glBegin(GL_POINTS);
	for (int i=0; i < (int)path.m_points.size(); i++)
		glVertex3fv(&path.m_points[i].x);
	glEnd();

	glPopAttrib();
}

//------------------------------------------------------------------------

PathSolution::PathSolution(const Room&		room,
						   const Source&	source,
						   const Listener&	listener,
						   int				maximumOrder)
:	m_room			(room),
	m_source		(source),
	m_listener		(listener),
	m_maximumOrder	(maximumOrder)
{
	m_polygonCache.resize(maximumOrder);
	m_validateCache.resize(maximumOrder*2);
}

PathSolution::~PathSolution(void)
{
	// empty
}

//------------------------------------------------------------------------

void PathSolution::clearCache(void)
{
	m_solutionNodes.clear();
	m_failPlanes.clear();
}

//------------------------------------------------------------------------

void PathSolution::update(void)
{
	int numProc   = 0;
	int numTested = 0;

	Vector3 source = m_source.getPosition();
	Vector3 target = m_listener.getPosition();

	m_paths.clear();

	if (!m_solutionNodes.size() || m_cachedSource != source || GLUT::getKey('p'))
	{
		clearCache();

		SolutionNode root;
		root.m_polygon = 0;
		root.m_parent  = -1;
		m_solutionNodes.push_back(root);

		solveRecursive(source, target, Beam(), 0, 0);

		m_cachedSource = source;
		EL_ASSERT(m_solutionNodes.size() == m_failPlanes.size());

		int numBuckets = (m_solutionNodes.size() + DISTANCE_SKIP_BUCKET_SIZE - 1) / DISTANCE_SKIP_BUCKET_SIZE;
		m_distanceSkipCache.resize(numBuckets);
		for (int i=0; i < numBuckets; i++)
			m_distanceSkipCache[i].set(0,0,0,0);
	}

	int n  = m_solutionNodes.size();
	int nb = (m_solutionNodes.size() + DISTANCE_SKIP_BUCKET_SIZE - 1) / DISTANCE_SKIP_BUCKET_SIZE;
	const Vector4* skipSphere = &m_distanceSkipCache[0];
	for (int b=0; b < nb; b++)
	{
		const Vector4& fc = *skipSphere++;
		Vector3 r = target - Vector3(fc.x, fc.y, fc.z);
		if (r.lengthSqr() < fc.w)
			continue;

		float maxdot = 0.f;
		numProc++;

		int imn = b * DISTANCE_SKIP_BUCKET_SIZE;
		int imx = imn + DISTANCE_SKIP_BUCKET_SIZE;
		if (imx > n)
			imx = n;
		for (int i=imn; i < imx; i++)
		{
			float d = dot(target, m_failPlanes[i]);
			if (d >= 0.f)
			{
				t_validation.toggle();
				validatePath(source, target, i, m_failPlanes[i]);
				t_validation.toggle();
				numTested++;
			}
			if (i == imn || d > maxdot)
				maxdot = d;
		}

		if (maxdot < 0.f)
			m_distanceSkipCache[b].set(target.x, target.y, target.z, maxdot*maxdot);
	}

	m_pathFirstSet.clear();

	printf("paths: %d (proc %d = %.2f %%, tested %d, valid %d)\n", m_solutionNodes.size(), numProc * DISTANCE_SKIP_BUCKET_SIZE, (float)numProc/nb*100.f, numTested, m_paths.size());
}

Vector4 PathSolution::getFailPlane(const Beam& beam, const Vector3& target)
{
	Vector4 failPlane(0.f, 0.f, 0.f, 1.f);
	if (beam.numPleqs() > 0)
		failPlane = beam.getPleq(0);
	for (int i=1; i < beam.numPleqs(); i++)
		if (dot(target, beam.getPleq(i)) < dot(target, failPlane))
			failPlane = beam.getPleq(i);

	return failPlane;
}

void PathSolution::validatePath(const Vector3& source,
								const Vector3& target,
								int nodeIndex,
								Vector4& failPlane)
{
	// collect polygons
	int order = 0;
	while(nodeIndex)
	{
		m_polygonCache[order++] = m_solutionNodes[nodeIndex].m_polygon;
		nodeIndex = m_solutionNodes[nodeIndex].m_parent;
	}

	// reconstruct image source
	Vector3 imgSource = source;
	for (int i=order-1; i >= 0; i--)
		imgSource = mirror(imgSource, m_polygonCache[i]->getPleq());

	// test for polygon miss and failed reflection
	Vector3 s = imgSource;
	Vector3 t = target;

	bool		   missed	 = false;
	int			   missOrder = -1;
	const Polygon* missPoly  = 0;
	Ray			   missRay(Vector3(0,0,0), Vector3(0,0,0));
	bool		   missSide  = false;

	for (int i=0; i < order; i++)
	{
		const Polygon* poly = m_polygonCache[i];
		const Vector4& pleq = poly->getPleq();
		Ray ray(s, t);

		// on same side of polygon?
		if (dot(s, pleq) * dot(t, pleq) > 0.f)
		{
			missed	  = true;
			missSide  = true;
			missOrder = i;
			missPoly  = poly;
			missRay	  = ray;
			break;
		}
		
		if (!ray.intersectExt(*poly))
		{
			missed	  = true;
			missSide  = false;
			missOrder = i;
			missPoly  = poly;
			missRay   = ray;
			break;
		}

		Vector3 isect = intersect(ray, pleq);
		s = mirror(s, pleq);
		t = isect;

		m_validateCache[i*2] = isect;
		m_validateCache[i*2+1] = s;
	}

	if (missed)
	{
		Vector4 missPlane(0.f, 0.f, 0.f, 0.f);
		if (missSide)
		{
			// reconstruct from plane equation
			missPlane = missPoly->getPleq();
			if (dot(missRay.m_a, missPlane) > 0.f)
				missPlane = -missPlane;
		} else
		{
			// reconstruct from missed polygon edge
			Beam beam(missRay.m_a, *missPoly);
			missPlane = beam.getPleq(1);
			for (int i=2; i < beam.numPleqs(); i++)
				if (dot(missRay.m_b, beam.getPleq(i)) < dot(missRay.m_b, missPlane))
					missPlane = beam.getPleq(i);
		}
		
		// mirror the miss plane according to the remaining polygons
		for (int i=missOrder-1; i >= 0; i--)
			missPlane = mirror(missPlane, m_polygonCache[i]->getPleq());

		// because of numerical inaccuracies, we may end up on wrong side
		if (dot(target, missPlane) > 0.f)
		{
			// reconstruct beam for robust fail plane construction
			Beam beam;
			imgSource = source;
			for (int i=order-1; i >= 0; i--)
			{
				Polygon poly = *m_polygonCache[i];
				poly.clip(beam);

				imgSource = mirror(imgSource, poly.getPleq());
				beam = Beam(imgSource, poly);
			}

			// update fail plane
			missPlane = getFailPlane(beam, target);
		}

		// done, normalize to be sure
		failPlane = normalize(missPlane);
		return;
	}

	// test for occlusion
	t = target;
	for (int i=0; i < order; i++)
	{
		Vector3 isect = m_validateCache[i*2];
		if (m_room.getBSP().rayCastAny(Ray(isect, t)))
			return;

		t = isect;
	}
	if (m_room.getBSP().rayCastAny(Ray(source, t)))
		return;

	// validated, add to results
	Path path;
	path.m_order = order;
	path.m_points.resize(order+2);
	path.m_polygons.resize(order);

	t = target;
	for (int i=0; i < order; i++)
	{
		path.m_points[order-i+1] = t;
		path.m_polygons[order-i-1] = m_polygonCache[i];

		t = m_validateCache[i*2];
	}

	path.m_points[0] = source;
	path.m_points[1] = t;

	// finally remove similar paths to dodge bad geometry

	float fval = dot(path.m_points[1], Vector3(1.f, 1.f, 1.f));
	float fmin = fval - 2.f*EPS_SIMILAR_PATHS;
	float fmax = fval + 2.f*EPS_SIMILAR_PATHS;

	for(std::multimap<float, int>::const_iterator it = m_pathFirstSet.upper_bound(fmin);
		it != m_pathFirstSet.end() && it->first <= fmax;
		++it)
	{
		int i = it->second;

		const Path& p = m_paths[i];
		if (p.m_order != order)
			continue;
		bool safe = false;
		for (int j=1; j < (int)p.m_points.size()-1; j++)
		{
			if ((p.m_points[j] - path.m_points[j]).lengthSqr() > EPS_SIMILAR_PATHS * EPS_SIMILAR_PATHS)
			{
				safe = true;
				break;
			}
		}
		if (!safe)
			return;
	}

	m_pathFirstSet.insert(std::pair<float,int>(fval, m_paths.size()));
	m_paths.push_back(path);
}

void PathSolution::solveRecursive(const Vector3& source,
								  const Vector3& target,
								  const Beam& beam,
								  int order,
								  int parentIndex)
{
	// start with the optimal fail plane
	m_failPlanes.push_back(getFailPlane(beam, target));

	if (order >= m_maximumOrder)
		return;

	std::vector<const Polygon*> polygons;
	m_room.getBSP().beamCast(beam, polygons);
	for (int i=(int)polygons.size()-1; i >= 0; i--)
	{
		const Polygon* orig = polygons[i];
		Vector3 imgSource = mirror(source, orig->getPleq());

		if (parentIndex > 0)
		{
			const Polygon* ppoly = m_solutionNodes[parentIndex].m_polygon;
			if (orig == ppoly)
				continue;

			Vector3 testSource = mirror(imgSource, ppoly->getPleq());
			if ((source-testSource).length() < EPS_SIMILAR_PATHS)
				continue;
		}

		Polygon poly = *orig;
		if (poly.clip(beam) == Polygon::CLIP_VANISHED)
			continue;

		if (poly.getArea() < EPS_DEGENERATE_POLYGON_AREA)
			continue;

		Beam b(imgSource, poly);

		// create new solution node
		SolutionNode node;
		node.m_polygon = orig;
		node.m_parent  = parentIndex;
		m_solutionNodes.push_back(node);

		solveRecursive(imgSource, target, b, order+1, m_solutionNodes.size()-1);

		if (order==0)
			printf("building beam tree.. %.2f %% (%d)\r", 100.f-(float)i/(float)polygons.size()*100.f, m_solutionNodes.size());
	}

	if (order==0)
		printf("\n");
}
