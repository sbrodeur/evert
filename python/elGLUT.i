/* File: elGLUT.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elGLUT.hpp"
%}

//------------------------------------------------------------------------
namespace EL {
//--------------------------------------------------------------------

class GLUT
{
public:
	static void				start			(void(*displayFunc)(void), int width=640, int height=480);

	static const Matrix3x4&	getCameraMatrix	(void)						{ return cameraMtx; }
	static void				setCameraMatrix	(const Matrix3x4& m)		{ cameraMtx = m; }
	static void				loadCameraMatrix(void);
	
	static float			getAspectRatio	(void)						{ return (float)W_WIDTH/W_HEIGHT; }
	static int				numPixels		(void)						{ return W_WIDTH*W_HEIGHT; }
	static int				getWidth		(void)						{ return W_WIDTH; }
	static int				getHeight		(void)						{ return W_HEIGHT; }
	static bool				getKey			(char key)					{ return !!kbdBuffer[key]; }
	static float			getMovementSpeed(void)						{ return movementSpeed; }
	static void				setMovementSpeed(float speed)				{ movementSpeed = speed; }

	//--------------------------------------------------------------------

	static void				loadMatrix		(const Matrix3x4& m);
	static void				multMatrix		(const Matrix3x4& m);

	enum AlignMode
	{
		ALIGN_LEFT		= 0x01,
		ALIGN_RIGHT		= 0x02,
		ALIGN_TOP		= 0x04,
		ALIGN_BOTTOM	= 0x08,
	};
	static void				renderString3D	(const Vector3& pos,
											 const char* string,
											 const Vector3& color,
											 const Vector4 bgColor = Vector4(0.f, 0.f, 0.f, 0.f),
											 float scale = 1.f,
											 int align = 0);

	//--------------------------------------------------------------------

private:
	static void 	kbdfunc					(unsigned char key, int, int);
	static void  kbdupfunc				(unsigned char key, int, int);
	static void  specialfunc				(int key, int, int);
	static void  specialupfunc			(int key, int, int);
	static void  idlefunc				(void);
	static void  reshape					(int w, int h);
	static void  glutpassivemotionfunc	(int x, int y);
	static void  glutmotionfunc			(int x, int y);
	static void  mousefunc				(int button, int state,int,int);
	static void  display					(void);
	static void			motion					(int x, int y);

	static bool	left;
	static bool right;
	static bool middle;
	static bool shiftDown;
	static float movementSpeed;

	static int	lastx;
	static int	lasty;
	static int	glutx;
	static int	gluty;
	static int	W_WIDTH;
	static int	W_HEIGHT;

	static char kbdBuffer[256];
	static char kbdBuffer2[256];

	static Matrix3x4 cameraMtx;
	static void (*dfunc)(void);
};

//--------------------------------------------------------------------
} // namespace EL
