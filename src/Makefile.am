AM_CPPFLAGS = -Wall -I$(top_srcdir)/include
AM_LDFLAGS = -Wall -I$(top_srcdir)/src/.libs

lib_LTLIBRARIES=libevert.la
libevert_la_SOURCES = elBeam.cpp elBSP.cpp elGLUT.cpp elListener.cpp elOrientedPoint.cpp elPathSolution.cpp elPolygon.cpp elRay.cpp elRoom.cpp elSource.cpp elTimer.cpp elVector.cpp
libevert_la_CPPFLAGS = $(AM_CPPFLAGS)
libevert_la_LDFLAGS =-evert -version-info 1:0:0 -shared $(AM_LDFLAGS)
libevert_la_LIBADD = -lGL -lGLU -lglut #$(GL_LIBS) $(GLU_LIBS) $(GLUT_LIBS)

bin_PROGRAMS = evert
evert_SOURCES = eaMain.cpp 
evert_CPPFLAGS = $(AM_CPPFLAGS)
evert_LDFLAGS = $(AM_LDFLAGS)
#FIXME: there seems to be a problem with using $(GLU_LIBS), it seems empty
evert_LDADD = -lGL -lGLU -lglut libevert.la #$(GL_LIBS) $(GLU_LIBS) $(GLUT_LIBS)
