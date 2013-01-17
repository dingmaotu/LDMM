#ifndef _CUBEPBC_H
#define _CUBEPBC_H

#include <string>
using namespace std;

#include "ldmm.h"
#include "psp.h"
#include "pbc.h"
#include "particle.h"

class CubePBC: public PBC
{
    public:
        CubePBC(REAL L)
            :_L(L)
        {}
        REAL Dist2(Particle& p1, Particle& p2);
        REAL Dist(Particle& p1, Particle& p2, int k);
        void UpdateCoord();
        void InitPos(string);
        void SetPSP(PSP *psp){_psp = psp;}
        inline REAL GetL();
        inline REAL GetV();
    private:
        inline REAL Restrict(REAL r);
        inline REAL RestrictX(REAL r);
        void SetupFCC();
        void SetupCubic();
        PSP *_psp;
        REAL _L;
};

#endif
