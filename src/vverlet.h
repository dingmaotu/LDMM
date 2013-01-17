#ifndef _VVERLET_H
#define _VVERLET_H

#include "ldmm.h"
#include "psp.h"
#include "lj12_6.h"
#include "pbc.h"


class ITRVVerlet
{
    public:
        ITRVVerlet(PSP &, PBC *pbc, FFLJ12_6 *ff, REAL dt);
        ~ITRVVerlet(){}
        void UpdatePosAndVel();

        void InitVel(REAL, bool);

        REAL GetInstT();
        REAL GetInstP();
        REAL GetPotential();
        REAL GetKinetic();

        void RescaleV(REAL , REAL);

    private:
        void UpdatePos();
        void UpdateVel();
        void SetMV2();

        PSP &_psp;
        PBC *_pbc;
        FFLJ12_6 *_ff;
        REAL _dt;

        REAL _mv2;
};

#endif
