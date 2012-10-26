#!/usr/bin/env python

import conf.gromacs as gro
import sys

if __name__ == '__main__':
    a = gro.Conf()
    a.load(sys.argv[1])
    print "Saving to %s"%(sys.argv[2])
    a.saveTopol(sys.argv[2])
