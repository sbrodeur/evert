/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#define GLUT_DISABLE_ATEXIT_HACK
#include "elGLUT.hpp"

#include <unistd.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace EL;

bool GLUT::left		= false;
bool GLUT::right	= false;
bool GLUT::middle	= false;
bool GLUT::shiftDown= false;

int GLUT::lastx = 0;
int GLUT::lasty = 0;
int GLUT::glutx = 0;
int GLUT::gluty = 0;
int GLUT::W_WIDTH = 0;
int GLUT::W_HEIGHT = 0;
float GLUT::movementSpeed = 1.f;

char GLUT::kbdBuffer[256] = {0};
char GLUT::kbdBuffer2[256] = {0};

Matrix3x4 GLUT::cameraMtx;
void (*GLUT::dfunc)(void) = 0;

//------------------------------------------------------------------------

void  GLUT::kbdfunc(unsigned char key, int, int)
{
	if( key == 27 )
		exit(0);	//esc or alt-F4 exits, TODO make exit so that destructors are called
	if( key == 'p' )
	{
		display();
	}

	kbdBuffer[key] = 1;
	shiftDown = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}
void  GLUT::kbdupfunc(unsigned char key, int, int) 
{
	kbdBuffer[key] = 0; 
}
void  GLUT::specialfunc(int key, int, int)
{

	if( key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT )
		exit(0);	//esc or alt-F4 exits, TODO make exit to call destructors

	kbdBuffer2[key] = 1;
}
void  GLUT::specialupfunc(int key, int, int) { kbdBuffer2[key] = 0; }
void  GLUT::idlefunc() { glutPostRedisplay(); }
void  GLUT::reshape( int w, int h ) { glViewport( 0, 0, w, h ); W_WIDTH=w; W_HEIGHT=h; }

void  GLUT::glutpassivemotionfunc(int x, int y)
{
	glutx = x;
	gluty = y;
}

void  GLUT::glutmotionfunc(int x, int y)
{
	glutx = x;
	gluty = y;
}

void  GLUT::mousefunc(int button, int state,int,int)
{
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			left = (state == GLUT_DOWN);
			break;

		case GLUT_RIGHT_BUTTON:
			right = (state == GLUT_DOWN);
			break;

		case GLUT_MIDDLE_BUTTON:
			middle = (state == GLUT_DOWN);
			break;
	}
}

void GLUT::motion (int x, int y)
{
	float deltax = x-lastx;
	float deltay = y-lasty;

	if (left)
	{
		Matrix3x4 rotation;
		rotation.rotate(RAD(deltax), Vector3(0, 1, 0));
		rotation.rotate(RAD(deltay), Vector3(1, 0, 0));
		cameraMtx = rotation * cameraMtx;
	} else
	{
		Vector3 translation(movementSpeed * deltax * .02f, -movementSpeed * deltay * .02f, 0.f);
		cameraMtx.translate(translation);
	} 
}

void  GLUT::display()
{
	if (left||middle)
	{
		motion(glutx,gluty);
		glutSetCursor(GLUT_CURSOR_NONE);
		//FIXME: need to set cursor position in X11		
		//SetCursorPos(lastx,lasty);
	} else
	{
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		lastx = glutx;
		lasty = gluty;
	}

	Matrix3x4 rotation;
	Vector3   translation(0.f, 0.f, 0.f);

	float ROTATE_SPEED = (PI/180.0f);

	if (kbdBuffer2[GLUT_KEY_LEFT])  rotation.rotate(-ROTATE_SPEED, Vector3(0, 1, 0));
	if (kbdBuffer2[GLUT_KEY_RIGHT]) rotation.rotate(+ROTATE_SPEED, Vector3(0, 1, 0));
	if (kbdBuffer2[GLUT_KEY_UP])    rotation.rotate(-ROTATE_SPEED, Vector3(1, 0, 0));
	if (kbdBuffer2[GLUT_KEY_DOWN])  rotation.rotate(+ROTATE_SPEED, Vector3(1, 0, 0));
	if (kbdBuffer['q'])				rotation.rotate(-ROTATE_SPEED*4, Vector3(0, 0, 1));
	if (kbdBuffer['e'])				rotation.rotate(+ROTATE_SPEED*4, Vector3(0, 0, 1)); 

	float STRAFE_SPEED = shiftDown || kbdBuffer[' '] ? 500 : 100;
	STRAFE_SPEED *= 0.0015f * movementSpeed;

	if (kbdBuffer2[GLUT_KEY_PAGE_UP])     translation.y -= STRAFE_SPEED;
	if (kbdBuffer2[GLUT_KEY_PAGE_DOWN])	  translation.y += STRAFE_SPEED;
	if (kbdBuffer['w'] || kbdBuffer['W']) translation.z += STRAFE_SPEED;
	if (kbdBuffer['s'] || kbdBuffer['S']) translation.z -= STRAFE_SPEED;
	if (kbdBuffer['a'] || kbdBuffer['A']) translation.x += STRAFE_SPEED;
	if (kbdBuffer['d'] || kbdBuffer['D']) translation.x -= STRAFE_SPEED;

	cameraMtx = rotation * cameraMtx;
	cameraMtx.translate(translation);

	dfunc();
	glutSwapBuffers();
	usleep(1000);
}

//------------------------------------------------------------------------

void GLUT::loadMatrix(const Matrix3x4& m)
{
	float mtx[16];
	*((Matrix3x4*)mtx) = m;
	mtx[12] = mtx[3];
	mtx[13] = mtx[7];
	mtx[14] = mtx[11];
	mtx[15] = 1.f;
	mtx[3] = mtx[7] = mtx[11] = 0.f;

	swap(mtx[1], mtx[4]);
	swap(mtx[2], mtx[8]);
	swap(mtx[6], mtx[9]);

	glLoadMatrixf(mtx);
}

void GLUT::multMatrix(const Matrix3x4& m)
{
	float mtx[16];
	*((Matrix3x4*)mtx) = m;
	mtx[12] = mtx[3];
	mtx[13] = mtx[7];
	mtx[14] = mtx[11];
	mtx[15] = 1.f;
	mtx[3] = mtx[7] = mtx[11] = 0.f;

	swap(mtx[1], mtx[4]);
	swap(mtx[2], mtx[8]);
	swap(mtx[6], mtx[9]);

	glMultMatrixf(mtx);
}

void GLUT::loadCameraMatrix(void)
{
	loadMatrix(getCameraMatrix());
}

//------------------------------------------------------------------------

void GLUT::start(void(*displayFunc)(void), int width, int height)
{
	W_WIDTH  = width;
	W_HEIGHT = height;
	dfunc    = displayFunc;

	char param[] = "GLUT";
	char *argv[] = { param, NULL };
	int argc = 1;
	glutInit( &argc, argv );

	glutInitDisplayString	("rgb double depth>=16");
	glutInitWindowSize		(W_WIDTH, W_HEIGHT);
	glutCreateWindow		("EVERT");
	glutDisplayFunc			(display);
	glutSetKeyRepeat		(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc		(kbdfunc);
	glutSpecialFunc			(specialfunc);
	glutKeyboardUpFunc		(kbdupfunc);
	glutSpecialUpFunc		(specialupfunc);
	glutReshapeFunc			(reshape);
	glutIdleFunc			(idlefunc);
	glutMouseFunc			(mousefunc);
	glutPassiveMotionFunc	(glutpassivemotionfunc);
	glutMotionFunc			(glutmotionfunc);

	glutMainLoop();
}

//------------------------------------------------------------------------

void GLUT::renderString3D(const Vector3& pos, const char* string, const Vector3& color, const Vector4 bgColor, float scale, int align)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
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

	// render string
	Matrix3x4 mtx = getCameraMatrix();

	Vector3 camPos = mtx * pos;
	float scl = -camPos.z * .001f;
	if (scl > 4.f)
		scl = 4.f;

	scl *= scale;

	mtx.translate(mtx.getRotation() * pos);
	mtx.setRotation(Matrix3(scl,0,0,0,scl,0,0,0,scl));
	
	if (align & ALIGN_RIGHT)
		mtx.translate(Vector3(-scl*width, 0.f, 0.f));
	else if (!(align & ALIGN_LEFT))
		mtx.translate(Vector3(-scl*width/2, 0.f, 0.f));

	if (align & ALIGN_TOP)
		mtx.translate(Vector3(0.f, -scl*80.f, 0.f));
	else if (!(align & ALIGN_BOTTOM))
		mtx.translate(Vector3(0.f, -scl*40.f, 0.f));

	GLUT::loadMatrix(mtx);

	static const float miny = -33.33f;
	static const float maxy = 119.05f;
	static const float pad	= 15.f;

	glColor4fv(&bgColor.x);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glVertex3f(-pad, miny-pad, 0.f);
	glVertex3f(width+pad, miny-pad, 0.f);
	glVertex3f(width+pad, maxy+pad, 0.f);
	glVertex3f(-pad, maxy+pad, 0.f);
	glEnd();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
/*	glColor3fv(&color.x);
	glLineWidth(1.f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-pad, miny-pad, 0.f);
	glVertex3f(width+pad, miny-pad, 0.f);
	glVertex3f(width+pad, maxy+pad, 0.f);
	glVertex3f(-pad, maxy+pad, 0.f);
	glEnd();
*/
	glColor3fv(&color.x);
	glLineWidth(2.f);
	for (const char* p = string; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);

	glPopMatrix();
	glPopAttrib();
}
