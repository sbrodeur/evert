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

	if (!g_solution)
		g_solution = new PathSolution(g_room, g_room.getSource(0), g_room.getListener(0), 5);

	g_solution->update();

	for (int i=0; i < g_solution->numPaths(); i++)
		g_solution->renderPath(g_solution->getPath(i));
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
