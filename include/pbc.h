#ifndef _PBC_H
#define _PBC_H

#include <string>
#include "ldmm.h"
#include "particle.h"
class PBC
{
    public:
        virtual ~PBC(){}
        virtual REAL Dist2(Particle&, Particle&)=0;
        virtual REAL Dist(Particle&, Particle&, int)=0;
        virtual void InitPos(std::string) = 0;
        virtual void SetPSP(PSP *) = 0;
        virtual void UpdateCoord() = 0;
        virtual REAL GetV() = 0;
        virtual REAL GetL() = 0;
};

#endif
