/*************************************************************************
 *
 * EVERT Application
 * -----------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elAABB.hpp"
#include "elBeam.hpp"
#include "elBSP.hpp"
#include "elGLUT.hpp"
#include "elPathSolution.hpp"
#include "elPolygon.hpp"
#include "elRay.hpp"
#include "elRoom.hpp"
#include "elTimer.hpp"
#include "elVector.hpp"
#include <cstdio>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace EL;

static const int G_WIDTH  = 640*2;
static const int G_HEIGHT = 480*2;

//------------------------------------------------------------------------

extern CPUTimer t_bsp;
extern CPUTimer t_polyClip;
extern CPUTimer t_validation;

Room			g_room;
PathSolution*	g_solution;

//------------------------------------------------------------------------

void display(void)
{
	// straighten camera matrix
	{
		Matrix3x4 mtx = GLUT::getCameraMatrix();
		mtx.invert();
		Vector3 dof   = normalize(mtx.getColumn(2));
		Vector3 right = normalize(cross(dof, Vector3(0, 0, 1)));
		Vector3 up    = -normalize(cross(dof, right));
		mtx.setColumn(0, right);
		mtx.setColumn(1, up);
		mtx.setColumn(2, dof);
		mtx.invert();
		GLUT::setCameraMatrix(mtx);
	}

	//--------------------------------------------------------------------
	// Frame initialization
	//--------------------------------------------------------------------

	// clear screen
	glClearColor(.1f, .2f, .4f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	// set projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, GLUT::getAspectRatio(), 10.f, 100000.f);

	// load camera matrix
	glMatrixMode(GL_MODELVIEW);
	GLUT::loadCameraMatrix();

	//--------------------------------------------------------------------
	// Tweaking the source
	//--------------------------------------------------------------------

	OrientedPoint* opt = &g_room.getListener(0);
	float speed = 40.f;
	if (GLUT::getKey(' ')) speed *= 10.f;
	if (GLUT::getKey('r')) opt->translate(Vector3( 0, 0, 1)*speed);
	if (GLUT::getKey('f')) opt->translate(Vector3( 0, 0,-1)*speed);
	if (GLUT::getKey('t')) opt->translate(Vector3(-1, 0, 0)*speed);
	if (GLUT::getKey('g')) opt->translate(Vector3( 1, 0, 0)*speed);
	if (GLUT::getKey('v')) opt->translate(Vector3( 0, 1, 0)*speed);
	if (GLUT::getKey('b')) opt->translate(Vector3( 0,-1, 0)*speed);

	//--------------------------------------------------------------------
	// Draw coordinate system
	//--------------------------------------------------------------------

	Vector3 mn, mx;
	g_room.getBoundingBox(mn, mx);
	Vector3 diag = mx-mn;
	mn -= diag*.1f;
	mx += diag*.1f;
	glLineWidth(3.f);
	glColor3f(.2f, .4f, .8f);
	glBegin(GL_LINES);
	glVertex3f(mn.x, mn.y, mn.z);	glVertex3f(mx.x, mn.y, mn.z);
	glVertex3f(mn.x, mn.y, mn.z);	glVertex3f(mn.x, mx.y, mn.z);
	glVertex3f(mn.x, mn.y, mn.z);	glVertex3f(mn.x, mn.y, mx.z);
	glEnd();
	GLUT::renderString3D(Vector3(mx.x, mn.y, mn.z), "x", Vector3(1,1,1), Vector4(0,0,0,0), 4.f);
	GLUT::renderString3D(Vector3(mn.x, mx.y, mn.z), "y", Vector3(1,1,1), Vector4(0,0,0,0), 4.f);
	GLUT::renderString3D(Vector3(mn.x, mn.y, mx.z), "z", Vector3(1,1,1), Vector4(0,0,0,0), 4.f);
	glLineWidth(1.f);

	//--------------------------------------------------------------------
	// Draw room, sources and listeners
	//--------------------------------------------------------------------

	g_room.render();
	for (int i=0; i < g_room.numSources(); i++)
		g_room.getSource(i).render(Vector3(0,1,0));
	for (int i=0; i < g_room.numListeners(); i++)
		g_room.getListener(i).render(Vector3(1,1,0));

	//--------------------------------------------------------------------
	// Solve 1st mirror rays
	//--------------------------------------------------------------------
#if 0
	glLineWidth(2.f);

	for (int i=0; i < g_room.numConvexElements(); i++)
	{
		const Polygon& poly = g_room.getConvexElement(i).m_polygon;

		Vector3 src  = g_room.getSource(0).getPosition();
		Vector3 lst  = g_room.getListener(0).getPosition();
		
		Vector3 msrc = mirror(src, poly.getPleq());

		Ray ray(msrc, lst);
		if (ray.intersect(poly))
		{
			Vector3 isect = intersect(ray, poly.getPleq());
			Ray ra(src, isect);
			Ray rb(isect, lst);

			glLineWidth(2.f);
			if (dot(src, poly.getPleq()) * dot(lst, poly.getPleq()) <= 0.f)
				glLineWidth(5.f);

			if (g_room.getBSP().rayCastAny(ra) ||
				g_room.getBSP().rayCastAny(rb))
				continue;

			ra.render(Vector3(1,1,0));
			rb.render(Vector3(1,1,0));
		}
	}

	//--------------------------------------------------------------------
	// Solve 2nd mirror rays
	//--------------------------------------------------------------------

	for (int i=0; i < g_room.numConvexElements(); i++)
	for (int j=0; j < g_room.numConvexElements(); j++)
	{
		if (i==j)
			continue;

		const Polygon& poly0 = g_room.getConvexElement(i).m_polygon;
		const Polygon& poly1 = g_room.getConvexElement(j).m_polygon;

		Vector3 src   = g_room.getSource(0).getPosition();
		Vector3 lst   = g_room.getListener(0).getPosition();
		
		Vector3 msrc1 = mirror(src, poly0.getPleq());
		Vector3 mlst1 = mirror(lst, poly1.getPleq());

		Ray ray(msrc1, mlst1);
		if (ray.intersect(poly0) && ray.intersect(poly1))
		{
			const Vector4& pleq0 = poly0.getPleq();
			const Vector4& pleq1 = poly1.getPleq();

			Vector3 isect0 = intersect(ray, pleq0);
			Vector3 isect1 = intersect(ray, pleq1);
			Ray ra(src, isect0);
			Ray rb(isect0, isect1);
			Ray rc(isect1, lst);

			if (dot(src, pleq0) * dot(isect1, pleq0) <= 0.f ||
				dot(isect0, pleq1) * dot(lst, pleq1) <= 0.f)
				continue;

			if (g_room.getBSP().rayCastAny(ra) ||
				g_room.getBSP().rayCastAny(rb) ||
				g_room.getBSP().rayCastAny(rc))
				continue;

			glLineWidth(1.f);
			ra.render(Vector3(1.f,.75f,0));
			rb.render(Vector3(1.f,.75f,0));
			rc.render(Vector3(1.f,.75f,0));
		}
	}
#endif
	//--------------------------------------------------------------------
	// Draw ray
	//--------------------------------------------------------------------
#if 0
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);

	static int pid = 46;
	const Polygon& poly = g_room.getConvexElement(pid).m_polygon;
	Beam beam(mirror(g_room.getSource(0).getPosition(), poly.getPleq()), poly);
	beam.render(Vector3(.125f,.25f,.125f));

	glDisable(GL_BLEND);
	glEnable(GL_POLYGON_STIPPLE);
	unsigned int spat[32];
	for (int i=0; i < 32; i++)
		spat[i] = 0x33333333 << (i&2);
	glPolygonStipple((const unsigned char*)spat);

/*	for (int i=0; i < g_room.numConvexElements(); i++)
	{
		Polygon poly = g_room.getConvexElement(i).m_polygon;
		if (poly.clip(beam) != Polygon::CLIP_VANISHED)
			poly.render(Vector3(0,1,1));
	}
*/
	std::vector<const Polygon*> polys;
	g_room.getBSP().beamCast(beam, polys);
	for (int i=0; i < (int)polys.size(); i++)
		polys[i]->render(Vector3(0,1,1));
	
	glDepthMask(GL_TRUE);
	glDisable(GL_POLYGON_STIPPLE);
#endif
/*
	Vector3 pt[7];
	for (int i=0; i < 7; i++)
	{
		pt[i].set(frand(), frand(), frand());
		pt[i] -= Vector3(.5f, .5f, .5f);
//		pt[i] *= 100.f;
	}

	Vector4 p0 = getPlaneEquation(pt[0], pt[1], pt[2]);
	Vector4 p1 = getPlaneEquation(pt[3], pt[4], pt[5]);
	Vector3 p = pt[6];
	float r0 = dot(mirror(p, p1), p0);
	float r1 = dot(p, mirror(p0, p1));
	printf("%f %f %f\n", r0, r1, r0-r1);
*/
#if 1
	CPUTimer t_total;
	Timer	 t_totalCalib;
	t_total.toggle();
	t_bsp.clear();
	t_polyClip.clear();
	t_validation.clear();

	if (!g_solution)
		g_solution = new PathSolution(g_room, g_room.getSource(0), g_room.getListener(0), 5);

	g_solution->update();

	t_total.toggle();
	t_total.calibrate(t_totalCalib);

	for (int i=0; i < g_solution->numPaths(); i++)
		g_solution->renderPath(g_solution->getPath(i));

	printf("total: %.6f s\n", t_total.get());
	printf("valid: %.6f s\n", t_validation.get());
	printf("bsp:   %.6f s\n", t_bsp.get());
	printf("pclip: %.6f s\n", t_polyClip.get());
#endif
}

//------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc < 1) {
		// Tell the user how to run the program
		printf("Usage: %s FILENAME \n", argv[0]);
		return 1;
	}
	const char* filename = argv[1];

	if (!g_room.import(filename)){
		printf("Failed to import room: %s \n", filename);
		return 1;
	}

	// setup initial camera position
	Matrix3x4 cameraMtx;
	cameraMtx.setRotation(Matrix3(0,0,1,-1,0,0,0,1,0));
	cameraMtx.translate(g_room.getCenter());
	GLUT::setCameraMatrix(invert(cameraMtx));

	GLUT::setMovementSpeed(g_room.getMaxLength() * .01f);
	GLUT::start(display, G_WIDTH, G_HEIGHT);
	return 0;
}
