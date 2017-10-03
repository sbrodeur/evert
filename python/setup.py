#!/usr/bin/env python

"""
setup.py file for installing Python bindings using SWIG
"""

from distutils.core import setup, Extension

evert_module = Extension('_evert',
                         define_macros = [('MAJOR_VERSION', '1'),
                                          ('MINOR_VERSION', '0')],
                         include_dirs = ['../include'],
                         sources=['evert_wrap.cxx',
                                  '../src/elBeam.cpp',
                                  '../src/elBSP.cpp',
                                  '../src/elGLUT.cpp',
                                  '../src/elListener.cpp',
                                  '../src/elOrientedPoint.cpp',
                                  '../src/elPathSolution.cpp',
                                  '../src/elPolygon.cpp',
                                  '../src/elRay.cpp',
                                  '../src/elRoom.cpp',
                                  '../src/elSource.cpp',
                                  '../src/elTimer.cpp', 
                                  '../src/elVector.cpp'],
                         libraries = ['GL', 'GLU', 'glut'],
                         library_dirs = [],
                        )

setup (name = 'evert',
       version = '1.0',
       author      = "Samuli Laine",
       description = """Accelerated beam tracing algorithm""",
       ext_modules = [evert_module],
       py_modules = ["evert"],
       )
