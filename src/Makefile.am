#FIXME: compilation needs to be done in 32bit mode only, seek why
AM_CPPFLAGS = -Wall -I$(top_srcdir)/include
AM_LDFLAGS = -Wall

bin_PROGRAMS = evert
evert_SOURCES = eaMain.cpp elBeam.cpp elBSP.cpp elGLUT.cpp elListener.cpp elOrientedPoint.cpp elPathSolution.cpp elPolygon.cpp elRay.cpp elRoom.cpp elSource.cpp elTimer.cpp elVector.cpp
evert_CPPFLAGS = $(AM_CPPFLAGS)
evert_LDFLAGS = $(AM_LDFLAGS)
#FIXME: there seems to be a problem with using $(GLU_LIBS), it seems empty
evert_LDADD = -lGL -lGLU -lglut #$(GL_LIBS) $(GLU_LIBS) $(GLUT_LIBS)
