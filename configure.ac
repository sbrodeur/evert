AC_INIT([evert], [1.0], [samuli.laine@tml.hut.fi])
AM_INIT_AUTOMAKE
AC_PROG_CXX

PKG_CHECK_MODULES(GL, [gl], [], [
		  AC_CHECK_HEADER([GL/gl.h],
		  		  [],
		  		  AC_MSG_ERROR([GL not found]))
		  AC_CHECK_LIB([GL],
			       [glBegin],
			       [GL_LIBS=-lGL],
			       AC_MSG_ERROR([GL required]))
])

PKG_CHECK_MODULES(GLU, [glu], [],
		  [AC_CHECK_HEADER([GL/glu.h],
				   [],
		  		   AC_MSG_ERROR([GLU not found]))
		   AC_CHECK_LIB([GLU],
				[gluBeginCurve],
				[GLU_LIBS=-lGLU],
			        AC_MSG_ERROR([GLU required])) ])
GLU_LIBS=-lGLU

PKG_CHECK_MODULES(GLUT, [glut], [],
		  [AC_CHECK_HEADER([GL/glut.h],
				   [],
		  		   AC_MSG_ERROR([GLUT not found]))
		   AC_CHECK_LIB([glut],
				[glutInit],
				[GLU_LIBS=-lglut],
			        AC_MSG_ERROR([GLUT required])) ])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
