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

import os
import math

from evert import Room, Source, Listener, Vector3, Matrix3, PathSolution, Viewer

CDIR = os.path.dirname(os.path.realpath(__file__))

def main():

    room = Room()
    
    # Load geometry from file
    filename = os.path.join(CDIR, '../../data/kuunteluhuone.room')
    room.importGeometry(filename)
    print('Using room geometry file: ', filename)
    
    center = room.getCenter()    
    print('Room maximum length: ', room.getMaxLength())
    print('Room center: x=%f, y=%f, z=%f' % (center.x, center.y, center.z))
    
    # Create source
    src1 = Source()
    src1.setPosition(Vector3(500,-250,1200))
    src1.setOrientation(Matrix3(0,0,1,1,0,0,0,1,0))
    src1.setName('Src1')
    room.addSource(src1)
    print('Number of sources: ', room.numSources())
    
    # Create listener
    list1 = Listener()
    list1.setPosition(Vector3(3750,225,1200))
    list1.setOrientation(Matrix3(0,0,-1,-1,0,0,0,1,0))
    list1.setName('Lst1')
    room.addListener(list1)
    print('Number of listeners: ', room.numListeners())
    
    for s in range(room.numSources()):
        for l in range(room.numListeners()):
            src = room.getSource(s)
            lst = room.getListener(l)
    
            print('-----------------------------------')
            print('From source %s to listener %s' % (src.getName(), lst.getName()))
            print('-----------------------------------')
            
            # Calculate paths
            maximumOrder = 5
            solution = PathSolution(room, src, lst, maximumOrder)
            solution.update()
            print('Number of paths calculated: ', solution.numPaths())
            
            # Analyze paths
            minPathLength = None
            maxPathLength = 0
            for i in range(solution.numPaths()):
                path = solution.getPath(i)
                
                # Calculate path length
                pathLength = 0.0
                lastPt = path.m_points[0]
                for pt in path.m_points[1:]:
                    pathLength += math.sqrt((lastPt.x - pt.x)**2 + 
                                            (lastPt.y - pt.y)**2 + 
                                            (lastPt.z - pt.z)**2)
                    lastPt = pt
                
                if pathLength > maxPathLength:
                    maxPathLength = pathLength
                if pathLength < minPathLength or minPathLength is None:
                    minPathLength = pathLength
                
            print('Minimum path length: ', minPathLength)
            print('Maximum path length: ', maxPathLength)

    viewer = Viewer(room, 4)
    viewer.show()

if __name__ == "__main__":
    main()