#!/usr/bin/env python
import conf.gromacs as gro
import sys


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print """
        Usage: gengro.py NP CellLength
        """
        sys.exit(1)

    NP = int(sys.argv[1])
    cellL = float(sys.argv[2])
    pList = [['SOL1', 'AR', NP/2], ['SOL2', 'NE', NP/2]]

    c = gro.Conf()
    c.multiInit(NP, pList)
    c.initPos(cellL)
    c.save("conf.gro")
    print "Box Length: %f; Cell Number: %d; Output to: conf.gro"%(c.box[0], c.box[0]/cellL)
