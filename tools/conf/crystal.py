# Several Predefined Cell Types
FCC = ((0.25, 0.25, 0.25),
       (0.75, 0.75, 0.25),
       (0.75, 0.25, 0.75),
       (0.25, 0.75, 0.75))

CUBE= ((0.5, 0.5, 0.5),)

def Pos(xN, yN, zN, cell=FCC):
    for x in xrange(xN):
        for y in xrange(yN):
            for z in xrange(zN):
                for p in cell:
                    yield (p[0]+x,
                           p[1]+y,
                           p[2]+z)

