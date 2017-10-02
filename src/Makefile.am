AM_CPPFLAGS = -Wall -O2 -I$(top_srcdir)/include
AM_LDFLAGS = 

bin_PROGRAMS = evert
evert_SOURCES = eaMain.cpp elBeam.cpp elBSP.cpp elGLUT.cpp elListener.cpp elOrientedPoint.cpp elPathSolution.cpp elPolygon.cpp elRay.cpp elRoom.cpp elSource.cpp elTimer.cpp elVector.cpp
evert_CPPFLAGS = $(AM_CPPFLAGS)
evert_LDFLAGS = $(AM_LDFLAGS)
evert_LDADD = -lGL -lGLU -lglut #$(GL_LIBS) $(GLU_LIBS) $(GLUT_LIBS)

