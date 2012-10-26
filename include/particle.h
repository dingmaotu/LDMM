#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "ldmm.h"

struct Particle
{
    int type;
    REAL x[DIM];
    REAL v[DIM];
    REAL f[DIM];
};

#endif
