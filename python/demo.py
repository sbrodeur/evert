#!/usr/bin/env python

from evert import Room, PathSolution

def main():

    filename = '../data/kuunteluhuone.room'
    g_room = Room()
    g_room._import(filename)

    print 'Using room file: ', filename
    print 'Number of sources: ', g_room.numSources()
    print 'Number of listeners: ', g_room.numListeners()
    
    opt = g_room.getListener(0)
    g_solution = PathSolution(g_room, g_room.getSource(0), g_room.getListener(0), 5)
    g_solution.update()
    
    print 'Number of paths calculated: ', g_solution.numPaths()
    
    maxOrder = 0
    for i in range(g_solution.numPaths()):
        path = g_solution.getPath(i)
        if path.m_order > maxOrder:
            maxOrder = path.m_order
    print 'Maximum order across all paths: ', maxOrder

if __name__ == "__main__":
    main()