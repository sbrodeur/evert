#!/usr/bin/env python

import math

from evert import Room, Source, Listener, Vector3, Matrix3, PathSolution

def main():

    room = Room()
    
    # Load geometry from file
    filename = '../../data/kuunteluhuone.room'
    room.importGeometry(filename)
    print 'Using room geometry file: ', filename
    
    center = room.getCenter()    
    print 'Room maximum length: ', room.getMaxLength()
    print 'Room center: x=%f, y=%f, z=%f' % (center.x, center.y, center.z)
    
    # Create source
    src1 = Source()
    src1.setPosition(Vector3(500,-250,1200))
    src1.setOrientation(Matrix3(0,0,1,1,0,0,0,1,0))
    src1.setName('Src1')
    room.addSource(src1)
    print 'Number of sources: ', room.numSources()
    
    # Create listener
    list1 = Listener()
    list1.setPosition(Vector3(3750,225,1200))
    list1.setOrientation(Matrix3(0,0,-1,-1,0,0,0,1,0))
    list1.setName('Lst1')
    room.addListener(list1)
    print 'Number of listeners: ', room.numListeners()
    
    for s in range(room.numSources()):
        for l in range(room.numListeners()):
            src = room.getSource(s)
            lst = room.getListener(l)
    
            print '-----------------------------------'
            print 'From source %s to listener %s' % (src.getName(), lst.getName())
            print '-----------------------------------'
            
            # Calculate paths
            maximumOrder = 5
            solution = PathSolution(room, src, lst, maximumOrder)
            solution.update()
            print 'Number of paths calculated: ', solution.numPaths()
            
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
                
            print 'Minimum path length: ', minPathLength
            print 'Maximum path length: ', maxPathLength

if __name__ == "__main__":
    main()