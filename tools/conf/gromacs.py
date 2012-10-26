class Particle:
    def __init__(self):
        self.residue = ''
        self.atom = ''
        self.pos = [0.0, 0.0, 0.0]
        self.residueNum = 0
        self.atomNum = 0

    def parse(self, parstr):
        try:
            self.residue = parstr[5:10].strip()
            self.atom = parstr[10:15].strip()
            self.pos[0] = float(parstr[20:28])
            self.pos[1] = float(parstr[28:36])
            self.pos[2] = float(parstr[36:44])
            self.atomNum = int(parstr[15:20])
            self.residueNum = int(parstr[0:5])
            return True
        except:
            return False

    def __str__(self):
        return '%5d%5s%5s%5d%8.3f%8.3f%8.3f'%(self.residueNum, self.residue, self.atom, self.atomNum, self.pos[0], self.pos[1], self.pos[2])

import os
class Conf:
    def __init__(self):
        self.title = 'Default'
        self.time  = 0.0
        self.hasTime = False
        self.NP = 0
        self.particles = []
        self.box = []

    def simpleInit(self, NP, residue, atom):
        if len(self.particles) > 0:
            self.particles = []
        self.NP = NP
        for i in xrange(NP):
            p = Particle()
            p.residue = residue
            p.atom = atom
            p.atomNum = i+1
            p.residueNum = i+1
            self.particles.append(p)

    def multiInit(self, NP, rs):
        """
        rs is the residue list, which has the
        format of [[residue, atom, Num], ...]
        """
        import random
        if len(self.particles) > 0:
            self.particles = []
        self.NP = NP
        nList = [i[2] for i in rs]
        if sum(nList) != NP:
            raise Exception("Number Not Match!");
        upperBound = len(nList) - 1
        for i in xrange(NP):
            idx = random.randint(0, upperBound)
            while nList[idx] == 0:
                idx = random.randint(0, upperBound)
            p = Particle()
            p.residue = rs[idx][0]
            p.atom = rs[idx][1]
            p.atomNum = i+1
            p.residueNum = i+1
            self.particles.append(p)
            nList[idx] -= 1

    def saveTopol(self, tFile):
        resList = []
        for i in xrange(self.NP):
            if i == 0:
                resList.append([self.particles[i].residue, 1])
                continue
            if resList[-1][0] == self.particles[i].residue:
                resList[-1][1] += 1
            else:
                resList.append([self.particles[i].residue, 1])
        f = open(tFile, 'w')
        for r in resList:
            f.write("%s %d\n"%(r[0],r[1]))
        f.close()

    def initPos(self, cellL, type='fcc'):
        import crystal as ct
        if self.NP > 0:
            m = 2
            while 4*m*m*m < self.NP:
                m = m + 1
            self.box = [m*cellL]*3
            celltype = getattr(ct, type.upper(),'FCC')
            cp = ct.Pos(m, m, m, celltype)
            for p in self.particles:
                p.pos = [cellL*x for x in cp.next()]

    def load(self, file):
        try:
            inFile = open(file, 'r')
            line = inFile.readline().strip().split('t=')
            self.title = line[0]
            if len(line) != 1:
                self.hasTime = True
                self.time = float(line[1])
            self.NP = int(inFile.readline())
            for i in range(self.NP): 
                p = Particle()
                if p.parse(inFile.readline()):
                    self.particles.append(p)
            t = inFile.readline().split()

            if len(t):
                self.box = [float(i) for i in t]

            inFile.close()
            return True
        except:
            return False

    def save(self, file, append=False):
        try:
            if append:
                outFile = open(file, 'a')
            else:
                outFile = open(file, 'w')

            outFile.write(self.title)
            if self.hasTime:
                outFile.write('%.3f'%self.time)
            outFile.write(os.linesep)
            outFile.write('%5d'%(self.NP)+os.linesep)
            for p in self.particles:
                outFile.write(str(p)+os.linesep)
            outFile.write(''.join(['%10.6f'%b for b in self.box])+os.linesep)
            outFile.close()
            return True
        except:
            return False
