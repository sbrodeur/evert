#ifndef __ELGLUT_HPP
#define __ELGLUT_HPP
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

#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

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

#endif // __ELGLUT_HPP
