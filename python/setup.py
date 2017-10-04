#!/usr/bin/env python

# Copyright (c) 2017, Simon Brodeur
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer.
#   
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
# OF SUCH DAMAGE.

"""
setup.py file for installing Python bindings using SWIG
"""

from distutils.core import setup, Extension

evert_module = Extension('_evert',
                         define_macros = [('MAJOR_VERSION', '1'),
                                          ('MINOR_VERSION', '0')],
                         include_dirs = ['../include'],
                         sources=['../src/elBeam.cpp',
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
                                  '../src/elVector.cpp',
                                  'evert.i'],
                         libraries = ['GL', 'GLU', 'glut'],
                         library_dirs = [],
                         language='c++',
                         swig_opts=['-c++', '-I../include'],
                         #extra_compile_args=['-std=c++11'],
                        )

setup (name = 'evert',
       version = '1.0',
       author      = "Samuli Laine",
       description = """Accelerated beam tracing algorithm""",
       ext_modules = [evert_module],
       py_modules = ["evert"],
       )
