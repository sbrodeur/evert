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

from __future__ import print_function
from evert import Room, Source, Listener, Vector3, Matrix3, Polygon, Viewer

def main():

    room = Room()
    
    # Define a simple cube (1000 x 1000 x 1000 mm) as room geometry
    face1poly = Polygon([Vector3(0,0,0), Vector3(0,1000,0), Vector3(1000,1000,0), Vector3(1000,0,0)], 1)
    face2poly = Polygon([Vector3(0,0,0), Vector3(0,1000,0), Vector3(0,1000,1000), Vector3(0,0,1000)])
    face3poly = Polygon([Vector3(0,0,0), Vector3(1000,0,0), Vector3(1000,0,1000), Vector3(0,0,1000)])
    face4poly = Polygon([Vector3(0,0,1000), Vector3(0,1000,1000), Vector3(1000,1000,1000), Vector3(1000,0,1000)])
    face5poly = Polygon([Vector3(0,1000,1000), Vector3(0,1000,0), Vector3(1000,1000,0), Vector3(1000,1000,1000)])
    face6poly = Polygon([Vector3(1000,0,1000), Vector3(1000,1000,1000), Vector3(1000,1000,0), Vector3(1000,0,0)])
    polygons = [face1poly, face2poly, face3poly, face4poly, face5poly, face6poly]
    for poly in polygons:
        room.addPolygon(poly, color=Vector3(0.5,0.5,0.5))
    room.constructBSP()

    center = room.getCenter()
    print('Room maximum length: ', room.getMaxLength())
    print('Room center: x=%f, y=%f, z=%f' % (center.x, center.y, center.z))
    print('Number of elements: ', room.numElements())
    print('Number of convex elements: ', room.numConvexElements())
    
    # Create source localized in room
    src1 = Source()
    src1.setPosition(Vector3(750,750,750))
    src1.setOrientation(Matrix3(0,0,1,
                                1,0,0,
                                0,1,0))
    src1.setName('Src1')
    room.addSource(src1)
    print('Number of sources: ', room.numSources())
    
    # Create listener localized in room
    list1 = Listener()
    list1.setPosition(Vector3(250, 250, 500))
    list1.setOrientation(Matrix3(0,0,-1,
                                 -1,0,0,
                                 0,1,0))
    list1.setName('Lst1')
    room.addListener(list1)
    
    list2 = Listener()
    list2.setPosition(Vector3(240, 240, 500))
    list2.setOrientation(Matrix3(0,0,-1,
                                 -1,0,0,
                                 0,1,0))
    list2.setName('Lst2')
    room.addListener(list2)
    print('Number of listeners: ', room.numListeners())
    
    viewer = Viewer(room, 4)
    viewer.show()
    
if __name__ == "__main__":
    main()